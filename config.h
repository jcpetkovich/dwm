/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int gappx     = 0;
static const unsigned int borderpx  = 10;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int minwsz    = 20;       /* Minimal heigt of a client for smfact */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#D84D39";
static const char col_cyan[]        = "#005577";
static const char *colors[][4]      = {
	/*               fg         bg         border     float */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray4, col_cyan },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     iscentered     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,             1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,             0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const float smfact    = 0.00; /* factor of tiled clients [0.00..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
#include "gaplessgrid.c"
#include "horizgrid.c"
#include "setsmfact.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },   /* mod-t */
	{ "><>",      NULL },   /* mod-f */
	{ "[M]",      monocle }, /* mod-m */
	{ "|M|",      centeredmaster }, /* mod-n */
	{ ">M>",      centeredfloatingmaster }, /* mod-shift-n */
	{ "TTT",      bstack },                 /* mod-a */
	{ "===",      bstackhoriz },            /* mod-shift-a */
	{ "[@]",      spiral },                 /* mod-s */
	{ "[\\]",     dwindle },                /* mod-shift-s */
	{ "###",      gaplessgrid }, /* mod-g */
	{ "#-#",      horizgrid },   /* mog-shift-g */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *termcmd[]  = {"term", "MUX", "STACK", "ATDIR", NULL};
static const char *emacscmd[]  = { "e", NULL };
static const char *statuscmd[]  = { "draw", "status", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

/* dmenu commands */
static const char *dmenucmd[] = {"dmenu", "run", NULL};
static const char *logoutcmd[] = { "dmenu", "halt", NULL };

static const char *dmenu_pass[] = {"dmenu", "pass", NULL};
/* static const char *dmenu_sys[] = {"dmenu", "sys", NULL}; */
/* static const char *dmenu_menu[] = {"dmenu", "menu", NULL}; */
static const char *dmenu_monitor[] = {"dmenu", "monitor", NULL};
static const char *dmenu_manpages[] = {"dmenu", "manpages", NULL};
static const char *dmenu_alarm[] = {"dmenu", "alarm", NULL};
static const char *dmenu_files[] = {"dmenu", "files", NULL};
static const char *dmenu_folders[] = {"dmenu", "folders", NULL};
static const char *dmenu_abook[] = {"dmenu", "abook", NULL};
static const char *dmenu_bookmarks[] = {"dmenu", "bookmarks", NULL};
static const char *dmenu_clipboard[] = {"dmenu", "clipboard", NULL};
static const char *dmenu_dict[] = {"dmenu", "dict", NULL};
static const char *dmenu_music[] = {"dmenu", "music", NULL};
/* static const char *dmenu_radio[] = {"dmenu", "radio", NULL}; */
static const char *dmenu_configs[] = {"dmenu", "configs", NULL};
static const char *dmenu_series[] = {"dmenu", "series", NULL};
static const char *dmenu_seriesa[] = {"dmenu", "seriesa", NULL};
static const char *dmenu_movies[] = {"dmenu", "movies", NULL};
static const char *dmenu_moviesa[] = {"dmenu", "moviesa", NULL};
static const char *dmenu_screensaver[] = {"dmenu", "screensaver", NULL};
static const char *dmenu_shistory[] = {"dmenu", "shistory", NULL};
static const char *dmenu_connect[] = {"dmenu", "connect", NULL};

static const char *volumeupcmd[] = { "volumeup", NULL };
static const char *volumedowncmd[] = { "volumedown", NULL };
static const char *togglemutecmd[] = { "amixer", "set", "Master", "toggle", NULL };
static const char *musicnextcmd[] = { "mpc", "next", NULL };
static const char *toggleplaycmd[] = { "mpc", "toggle", NULL };
static const char *screenshotcmd[] = { "scrot", NULL };

#include "movestack.c"
#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = emacscmd } },
	{ MODKEY,                       XK_o,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_q,      spawn,          {.v = statuscmd } },
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_q,      spawn,          {.v = logoutcmd } },

	/* change focus */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

	/* alter layout */
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setsmfact,      {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setsmfact,      {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_v,      zoom,           {0} },

	{ MODKEY,                       XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	/* layout selection */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_n,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_a,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_a,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY|ShiftMask,             XK_s,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[9]} },
	{ MODKEY|ShiftMask,             XK_g,      setlayout,      {.v = &layouts[10]} },

	/* HYPERKEYS */
	{ 0, XF86XK_AudioLowerVolume,              spawn,          {.v = volumedowncmd} },
	{ 0, XF86XK_AudioRaiseVolume,              spawn,          {.v = volumeupcmd} },
	{ 0, XF86XK_AudioMute,                     spawn,          {.v = togglemutecmd} },
	{ 0, XF86XK_AudioNext,                     spawn,          {.v = musicnextcmd} },
	{ 0, XF86XK_AudioPlay,                     spawn,          {.v = toggleplaycmd} },
	{ MODKEY|ControlMask,           XK_p,      spawn,          {.v = dmenu_pass} },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = screenshotcmd} },
	/* { MODKEY|ShiftMask,             XK_space,  spawn,          {.v = dmenu_sys} }, */
	/* { MODKEY,                       XK_space,  spawn,          {.v = dmenu_menu} }, */
	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = dmenu_monitor} },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = dmenu_manpages} },
	{ MODKEY|ControlMask,           XK_t,      spawn,          {.v = dmenu_alarm} },
	{ MODKEY|ControlMask,           XK_f,      spawn,          {.v = dmenu_files} },
	{ MODKEY|ControlMask,           XK_d,      spawn,          {.v = dmenu_folders} },
	{ MODKEY|ControlMask,           XK_a,      spawn,          {.v = dmenu_abook} },
	{ MODKEY|ControlMask,           XK_b,      spawn,          {.v = dmenu_bookmarks} },
	{ MODKEY|ControlMask,           XK_v,      spawn,          {.v = dmenu_clipboard} },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          {.v = dmenu_dict} },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = dmenu_music} },
	/* { MODKEY|ControlMask,           XK_r,      spawn,          {.v = dmenu_radio} }, */
	{ MODKEY|ControlMask,           XK_c,      spawn,          {.v = dmenu_configs} },
	{ MODKEY|ControlMask,           XK_s,      spawn,          {.v = dmenu_series} },
	{ MODKEY|ControlMask|ShiftMask, XK_s,      spawn,          {.v = dmenu_seriesa} },
	{ MODKEY|ControlMask,           XK_w,      spawn,          {.v = dmenu_movies} },
	{ MODKEY|ControlMask|ShiftMask, XK_w,      spawn,          {.v = dmenu_moviesa} },
	{ MODKEY|ControlMask,           XK_u,      spawn,          {.v = dmenu_screensaver} },
	{ MODKEY|ShiftMask,             XK_v,      spawn,          {.v = dmenu_shistory} },
	{ MODKEY|ControlMask,           XK_n,      spawn,          {.v = dmenu_connect} },

	{ MODKEY,                       XK_Tab,    view,           {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ControlMask,           XK_r,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,      movemouse,      {.i = 1} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button3,      resizemouse,    {.i = 1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
