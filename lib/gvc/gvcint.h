/* $Id$ $Revision$ */
/* vim:set shiftwidth=4 ts=8: */

/**********************************************************
*      This software is part of the graphviz package      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2004 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
**********************************************************/

/* Common header used by both clients and plugins */

#ifndef GVCINT_H
#define GVCINT_H

#ifdef __cplusplus
extern "C" {
#endif

#define ARRAY_SIZE(A) (sizeof(A)/sizeof(A[0]))

    typedef enum { PEN_NONE, PEN_DASHED, PEN_DOTTED, PEN_SOLID } pen_type;
    typedef enum { FILL_NONE, FILL_SOLID } fill_type;
    typedef enum { FONT_REGULAR, FONT_BOLD, FONT_ITALIC } font_type;
#define PENWIDTH_NORMAL 1
#define PENWIDTH_BOLD 2

    typedef struct {
	char *fontfam, fontopt;
	color_t pencolor, fillcolor;
	pen_type pen;
	fill_type fill;
	int penwidth;
	double fontsz;
    } gvstyle_t;

#define EMIT_SORTED (1<<0)
#define EMIT_COLORS (1<<1)
#define EMIT_CLUSTERS_LAST (1<<2)
#define EMIT_PREORDER (1<<3)
#define EMIT_EDGE_SORTED (1<<4)
#define GVRENDER_DOES_ARROWS (1<<5)
#define GVRENDER_DOES_LAYERS (1<<6)
#define GVRENDER_DOES_MULTIGRAPH_OUTPUT_FILES (1<<7)
#define GVRENDER_DOES_TRUECOLOR (1<<8)
#define GVRENDER_Y_GOES_DOWN (1<<9)
#define GVRENDER_X11_EVENTS (1<<10)

    typedef struct {
	int flags;
	int default_margin;
	int default_dpi;
	char **knowncolors;
	int sz_knowncolors;
	color_type_t color_type;
    } gvrender_features_t;

    typedef struct gvrender_job_s gvrender_job_t;

    struct gvrender_job_s {
	gvrender_job_t *next;
	char *output_filename;
	char *output_langname;
	FILE *output_file;
	int output_lang;

	gvrender_engine_t *render_engine;	/* current render engine */
	int render_id;		/* internal id of current render engine within plugin */
	gvrender_features_t *render_features;	/* features of current render */
#ifndef DISABLE_CODEGENS
	codegen_t *codegen;	/* current  codegen */
#endif

	void *surface;		/* gd or cairo surface */
	boolean external_surface; /* surface belongs to caller */

	gvstyle_t *style;       /* active style from gvc->styles[] */

	GVC_t *gvc;		/* parent gvc */
        graph_t *g;		/* parent graph */
        int flags;		/* emit_graph flags */

        unsigned int width;     /* width in pixels */
        unsigned int height;    /* height in pixels */
	int dpi;		/* resolution pixels-per-inch */
	int rot;		/* rotation */
	double zoom;		/* viewport zoom factor */
	pointf focus;		/* viewport focus in graph units */
//	pointf pointer;		/* pointer position in graph units */
//	boxf clip;		/* clip region in graph units */

	pointf compscale;	/* composite device scale incl: scale, zoom, dpi, y_goes_down */
	
        /* gvrender_begin_page */
	point pagesArrayElem;	/* 2D coord of current page - 0,0 based */
        int   pagesElem;	/* 1D index of current page - 0 based */
	point offset;

	/* gvrender_begin_layer() */
	int layer;
	int nLayers;

	boolean fit_mode, needs_refresh, click, active, has_grown;

	void *window;		/* display-specific data for gvrender plugin */
    };

/* gv_plugin_t is a descriptor for available plugins; gvplugin_t is for installed plugins */
    typedef struct gv_plugin_s gv_plugin_t;

    struct gv_plugin_s {
	gv_plugin_t *next;
	char *typestr;
	int quality;
	char *path;
	gvplugin_type_t *typeptr;
    };

#define MAXNEST 4

    struct GVC_s {
	/* gvNEWcontext() */
	char *user;
	char **info;

	/* gvrender_config() */
	gvrender_job_t *jobs;	/* linked list of jobs */
	gvrender_job_t *job;	/* current job */
	void (*errorfn) (char *fmt, ...);

	/* plugins */
#define ELEM(x) +1
	/* APIS expands to "+1 +1 ... +1" to give the number of APIs */
	gv_plugin_t *apis[ APIS ]; /* array of linked-list of plugins per api */
	gv_plugin_t *api[ APIS ];  /* array of current plugins per api */
#undef ELEM

	void *display;		/* display-specific data for gvdisplay plugin */
	gvdisplay_engine_t *display_engine; /* current display engine */

	gvtext_engine_t *text_engine;	/* current text_layout engine */

	/* gvrender_begin_job() */
	char *layout_type;      /* string name of layout type */
	gvlayout_engine_t *layout_engine;	/* current layout engine */
	int layout_id;		/* internal id of current layout */

	char **lib;

        /* pagination */
	point pagesArraySize;	/* 2D size of page array */
	point pagesArrayFirst;  /* 2D starting corner in */
	point pagesArrayMajor;	/* 2D major increment */
	point pagesArrayMinor;	/* 2D minor increment */
        int   pagesSize;	/* 1D size of page array */

	/* gvrender_begin_graph() */
	graph_t *g;

	box bb;			/* graph bounding box (what units???) */
	point pb;		/* page size - including margins (inches) */
	point margin;		/* page margins (inches) */

	gvstyle_t styles[MAXNEST]; /* style stack - reused by each job */
	int SP;

	/* render defaults set from graph */
	color_t bgcolor;	/* background color */
    };

#ifdef __cplusplus
}
#endif
#endif				/* GVCINT_H */
