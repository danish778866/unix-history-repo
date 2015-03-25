//===-- llvm/CodeGen/GCStrategy.h - Garbage collection ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// GCStrategy coordinates code generation algorithms and implements some itself
// in order to generate code compatible with a target code generator as
// specified in a function's 'gc' attribute. Algorithms are enabled by setting
// flags in a subclass's constructor, and some virtual methods can be
// overridden.
//
// GCStrategy is relevant for implementations using either gc.root or
// gc.statepoint based lowering strategies, but is currently focused mostly on
// options for gc.root.  This will change over time.
// 
// When requested by a subclass of GCStrategy, the gc.root implementation will
// populate GCModuleInfo and GCFunctionInfo with that about each Function in
// the Module that opts in to garbage collection.  Specifically:
// 
// - Safe points
//   Garbage collection is generally only possible at certain points in code.
//   GCStrategy can request that the collector insert such points:
//
//     - At and after any call to a subroutine
//     - Before returning from the current function
//     - Before backwards branches (loops)
// 
// - Roots
//   When a reference to a GC-allocated object exists on the stack, it must be
//   stored in an alloca registered with llvm.gcoot.
//
// This information can used to emit the metadata tables which are required by
// the target garbage collector runtime.
//
// When used with gc.statepoint, information about safepoint and roots can be
// found in the binary StackMap section after code generation.  Safepoint
// placement is currently the responsibility of the frontend, though late
// insertion support is planned.  gc.statepoint does not currently support
// custom stack map formats; such can be generated by parsing the standard
// stack map section if desired.
// 
// The read and write barrier support can be used with either implementation.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_GCSTRATEGY_H
#define LLVM_CODEGEN_GCSTRATEGY_H

#include "llvm/ADT/Optional.h"
#include "llvm/CodeGen/GCMetadata.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/Support/Registry.h"
#include <string>

namespace llvm {
  /// GCStrategy describes a garbage collector algorithm's code generation
  /// requirements, and provides overridable hooks for those needs which cannot
  /// be abstractly described.  GCStrategy objects currently must be looked up
  /// through the GCModuleInfo analysis pass.  They are owned by the analysis
  /// pass and recreated every time that pass is invalidated.
  class GCStrategy {
  private:
    std::string Name;
    friend class GCModuleInfo;
    
  protected:
    bool UseStatepoints;       /// Uses gc.statepoints as opposed to gc.roots,
                               /// if set, none of the other options can be
                               /// anything but their default values.

    unsigned NeededSafePoints; ///< Bitmask of required safe points.
    bool CustomReadBarriers;   ///< Default is to insert loads.
    bool CustomWriteBarriers;  ///< Default is to insert stores.
    bool CustomRoots;          ///< Default is to pass through to backend.
    bool CustomSafePoints;     ///< Default is to use NeededSafePoints
                               ///< to find safe points.
    bool InitRoots;            ///< If set, roots are nulled during lowering.
    bool UsesMetadata;         ///< If set, backend must emit metadata tables.
    
  public:
    GCStrategy();
    virtual ~GCStrategy() {}
    
    /// Return the name of the GC strategy.  This is the value of the collector
    /// name string specified on functions which use this strategy.
    const std::string &getName() const { return Name; }

    /// By default, write barriers are replaced with simple store
    /// instructions. If true, then performCustomLowering must instead lower
    /// them. 
    bool customWriteBarrier() const { return CustomWriteBarriers; }
    
    /// By default, read barriers are replaced with simple load
    /// instructions. If true, then performCustomLowering must instead lower
    /// them. 
    bool customReadBarrier() const { return CustomReadBarriers; }

    /// Returns true if this strategy is expecting the use of gc.statepoints,
    /// and false otherwise.
    bool useStatepoints() const { return UseStatepoints; }

    /** @name Statepoint Specific Properties */
    ///@{

    /// If the value specified can be reliably distinguished, returns true for
    /// pointers to GC managed locations and false for pointers to non-GC 
    /// managed locations.  Note a GCStrategy can always return 'None' (i.e. an
    /// empty optional indicating it can't reliably distinguish.   
    virtual Optional<bool> isGCManagedPointer(const Value *V) const {
      return None;
    }
    ///@}

    /** @name GCRoot Specific Properties
     * These properties and overrides only apply to collector strategies using
     * GCRoot. 
     */
    ///@{

    /// True if safe points of any kind are required. By default, none are
    /// recorded. 
    bool needsSafePoints() const {
      return CustomSafePoints || NeededSafePoints != 0;
    }
    
    /// True if the given kind of safe point is required. By default, none are
    /// recorded. 
    bool needsSafePoint(GC::PointKind Kind) const {
      return (NeededSafePoints & 1 << Kind) != 0;
    }
        
    /// By default, roots are left for the code generator so it can generate a
    /// stack map. If true, then performCustomLowering must delete them.
    bool customRoots() const { return CustomRoots; }

    /// By default, the GC analysis will find safe points according to
    /// NeededSafePoints. If true, then findCustomSafePoints must create them.
    bool customSafePoints() const { return CustomSafePoints; }
    
    /// If set, gcroot intrinsics should initialize their allocas to null
    /// before the first use. This is necessary for most GCs and is enabled by
    /// default. 
    bool initializeRoots() const { return InitRoots; }
    
    /// If set, appropriate metadata tables must be emitted by the back-end
    /// (assembler, JIT, or otherwise). For statepoint, this method is
    /// currently unsupported.  The stackmap information can be found in the
    /// StackMap section as described in the documentation.
    bool usesMetadata() const { return UsesMetadata; }

    ///@}
    
    /// initializeCustomLowering/performCustomLowering - If any of the actions
    /// are set to custom, performCustomLowering must be overriden to transform
    /// the corresponding actions to LLVM IR. initializeCustomLowering is
    /// optional to override. These are the only GCStrategy methods through
    /// which the LLVM IR can be modified.  These methods apply mostly to
    /// gc.root based implementations, but can be overriden to provide custom
    /// barrier lowerings with gc.statepoint as well.
    ///@{
    virtual bool initializeCustomLowering(Module &F) {
      // No changes made
      return false;
    }
    virtual bool performCustomLowering(Function &F) {
      llvm_unreachable("GCStrategy subclass specified a configuration which"
                       "requires a custom lowering without providing one");
    }
    ///@}
    /// Called if customSafepoints returns true, used only by gc.root
    /// implementations. 
    virtual bool findCustomSafePoints(GCFunctionInfo& FI, MachineFunction& MF) {
      llvm_unreachable("GCStrategy subclass specified a configuration which"
                       "requests custom safepoint identification without"
                       "providing an implementation for such");
    }
  };

  /// Subclasses of GCStrategy are made available for use during compilation by
  /// adding them to the global GCRegistry.  This can done either within the
  /// LLVM source tree or via a loadable plugin.  An example registeration
  /// would be:
  /// static GCRegistry::Add<CustomGC> X("custom-name",
  ///        "my custom supper fancy gc strategy");
  /// 
  /// Note that to use a custom GCMetadataPrinter w/gc.roots, you must also
  /// register your GCMetadataPrinter subclass with the
  /// GCMetadataPrinterRegistery as well. 
  typedef Registry<GCStrategy> GCRegistry;
}

#endif
