/*
 * $Id$
 */
static unsigned char ulaw_dsp[] = {
     0,    0,    0,    0,    0,    0,    0,    0, 
     0,    0,    0,    0,    0,    0,    0,    0, 
     0,    0,    0,    0,    0,    0,    0,    0, 
     0,    0,    0,    0,    0,    0,    0,    2, 
     5,    9,   13,   17,   21,   25,   29,   33, 
    37,   41,   45,   49,   53,   57,   61,   65, 
    68,   70,   72,   74,   76,   78,   80,   82, 
    84,   86,   88,   90,   92,   94,   96,   98, 
   100,  101,  102,  103,  104,  105,  106,  107, 
   108,  109,  110,  111,  112,  113,  114,  115, 
   115,  116,  116,  117,  117,  118,  118,  119, 
   119,  120,  120,  121,  121,  122,  122,  123, 
   123,  123,  124,  124,  124,  124,  125,  125, 
   125,  125,  126,  126,  126,  126,  127,  127, 
   127,  127,  127,  127,  128,  128,  128,  128, 
   128,  128,  128,  128,  128,  128,  128,  128, 
   255,  255,  255,  255,  255,  255,  255,  255, 
   255,  255,  255,  255,  255,  255,  255,  255, 
   255,  255,  255,  255,  255,  255,  255,  255, 
   255,  255,  255,  255,  255,  255,  255,  255, 
   252,  248,  244,  240,  236,  232,  228,  224, 
   220,  216,  212,  208,  204,  200,  196,  192, 
   189,  187,  185,  183,  181,  179,  177,  175, 
   173,  171,  169,  167,  165,  163,  161,  159, 
   157,  156,  155,  154,  153,  152,  151,  150, 
   149,  148,  147,  146,  145,  144,  143,  142, 
   142,  141,  141,  140,  140,  139,  139,  138, 
   138,  137,  137,  136,  136,  135,  135,  134, 
   134,  134,  133,  133,  133,  133,  132,  132, 
   132,  132,  131,  131,  131,  131,  130,  130, 
   130,  130,  130,  130,  129,  129,  129,  129, 
   129,  129,  129,  129,  128,  128,  128,  128, 
};

static unsigned char dsp_ulaw[] = {
    31,   31,   31,   32,   32,   32,   32,   33, 
    33,   33,   33,   34,   34,   34,   34,   35, 
    35,   35,   35,   36,   36,   36,   36,   37, 
    37,   37,   37,   38,   38,   38,   38,   39, 
    39,   39,   39,   40,   40,   40,   40,   41, 
    41,   41,   41,   42,   42,   42,   42,   43, 
    43,   43,   43,   44,   44,   44,   44,   45, 
    45,   45,   45,   46,   46,   46,   46,   47, 
    47,   47,   47,   48,   48,   49,   49,   50, 
    50,   51,   51,   52,   52,   53,   53,   54, 
    54,   55,   55,   56,   56,   57,   57,   58, 
    58,   59,   59,   60,   60,   61,   61,   62, 
    62,   63,   63,   64,   65,   66,   67,   68, 
    69,   70,   71,   72,   73,   74,   75,   76, 
    77,   78,   79,   81,   83,   85,   87,   89, 
    91,   93,   95,   99,  103,  107,  111,  119, 
   255,  247,  239,  235,  231,  227,  223,  221, 
   219,  217,  215,  213,  211,  209,  207,  206, 
   205,  204,  203,  202,  201,  200,  199,  198, 
   197,  196,  195,  194,  193,  192,  191,  191, 
   190,  190,  189,  189,  188,  188,  187,  187, 
   186,  186,  185,  185,  184,  184,  183,  183, 
   182,  182,  181,  181,  180,  180,  179,  179, 
   178,  178,  177,  177,  176,  176,  175,  175, 
   175,  175,  174,  174,  174,  174,  173,  173, 
   173,  173,  172,  172,  172,  172,  171,  171, 
   171,  171,  170,  170,  170,  170,  169,  169, 
   169,  169,  168,  168,  168,  168,  167,  167, 
   167,  167,  166,  166,  166,  166,  165,  165, 
   165,  165,  164,  164,  164,  164,  163,  163, 
   163,  163,  162,  162,  162,  162,  161,  161, 
   161,  161,  160,  160,  160,  160,  159,  159, 
};
