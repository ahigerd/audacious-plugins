/* FIXME: what to name this file? */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "paranormal.h"
#include "actuators.h"
#include "pn_utils.h"

/* **************** general_fade **************** */
static struct pn_actuator_option_desc general_fade_opts[] =
{
  { "amount", "The amount by which the color index of each "
    "pixel should be decreased by each frame (MAX 255)",
    OPT_TYPE_INT, { ival: 3 } },
  { NULL }
};

static void
general_fade_exec (const struct pn_actuator_option *opts,
	   gpointer data)
{
  int amt = opts[0].val.ival > 255 || opts[0].val.ival < 0 ? 3 : opts[0].val.ival;
  int i, j;

  for (j=0; j<pn_image_data->height; j++)
    for (i=0; i<pn_image_data->width; i++)
      pn_image_data->surface[0][PN_IMG_INDEX (i, j)] =
	CAPLO (pn_image_data->surface[0][PN_IMG_INDEX (i, j)]
	       - amt, 0);
}

struct pn_actuator_desc builtin_general_fade =
{
  "general_fade", "Fade-out", "Decreases the color index of each pixel",
  0, general_fade_opts,
  NULL, NULL, general_fade_exec
};

/* **************** general_blur **************** */
/* FIXME: add a variable radius */
/* FIXME: SPEEEED */
static void
general_blur_exec (const struct pn_actuator_option *opts,
	   gpointer data)
{
  int i,j;
  register guchar *srcptr = pn_image_data->surface[0];
  register guchar *destptr = pn_image_data->surface[1];
  register int sum;

  for (j=0; j<pn_image_data->height; j++)
    for (i=0; i<pn_image_data->width; i++)
      {
	sum = *(srcptr)<<2;

	/* top */
	if (j > 0)
	  {
	    sum += *(srcptr-pn_image_data->width)<<1;
	    if (i > 0)
	      sum += *(srcptr-pn_image_data->width-1);
	    if (i < pn_image_data->width-1)
	      sum += *(srcptr-pn_image_data->width+1);
	  }
	/* bottom */
	if (j < pn_image_data->height-1)
	  {
	    sum += *(srcptr+pn_image_data->width)<<1;
	    if (i > 0)
	      sum += *(srcptr+pn_image_data->width-1);
	    if (i < pn_image_data->width-1)
	      sum += *(srcptr+pn_image_data->width+1);
	  }
	/* left */
	if (i > 0)
	  sum += *(srcptr-1)<<1;
	/* right */
	if (i < pn_image_data->width-1)
	  sum += *(srcptr+1)<<1;

	*destptr++ = (guchar)(sum >> 4);
	srcptr++;
      }

  pn_swap_surfaces ();
}

struct pn_actuator_desc builtin_general_blur = 
{
  "general_blur", "Blur", "A simple 1 pixel radius blur",
  0, NULL,
  NULL, NULL, general_blur_exec
};

/* **************** general_mosaic **************** */
/* FIXME: add a variable radius */
/* FIXME: SPEEEED */
static struct pn_actuator_option_desc general_mosaic_opts[] =
{
  { "radius", "The pixel radius that should be used for the effect.",
    OPT_TYPE_INT, { ival: 6 } },
  { NULL }
};

static void
general_mosaic_exec (const struct pn_actuator_option *opts,
	   gpointer data)
{
  int i,j;
  register guchar *srcptr = pn_image_data->surface[0];
  register guchar *destptr = pn_image_data->surface[1];
  register int sum;
  int radius = opts[0].val.ival > 255 || opts[0].val.ival < 0 ? 6 : opts[0].val.ival;

  for (j=0; j<pn_image_data->height; j += radius)
    for (i=0; i<pn_image_data->width; i += radius)
      {
        int ii = 0, jj = 0;
        guchar bval = 0;

        /* find the brightest colour */
        for (jj = 0; jj < radius && (j + jj < pn_image_data->height); jj++)
          for (ii = 0; ii < radius && (i + ii < pn_image_data->width); ii++)
            {
               guchar val = srcptr[PN_IMG_INDEX(i + ii,  j + jj)];

               if (val > bval)
                 bval = val;
            }

        for (jj = 0; jj < radius && (j + jj < pn_image_data->height); jj++)
          for (ii = 0; ii < radius && (i + ii < pn_image_data->width); ii++)
            {
               destptr[PN_IMG_INDEX(i + ii, j + jj)] = bval;
            }
      }

  pn_swap_surfaces ();
}

struct pn_actuator_desc builtin_general_mosaic = 
{
  "general_mosaic", "Mosaic", "A simple mosaic effect.",
  0, general_mosaic_opts,
  NULL, NULL, general_mosaic_exec
};

/* **************** general_clear **************** */
static void
general_clear_exec (const struct pn_actuator_option *opts,
	   gpointer data)
{
   memset(pn_image_data->surface[0], '\0',
	  (pn_image_data->height * pn_image_data->width));
}

struct pn_actuator_desc builtin_general_clear =
{
  "general_clear", "Clear Surface", "Clears the surface.",
  0, NULL,
  NULL, NULL, general_clear_exec
};

/* **************** general_noop **************** */
static void
general_noop_exec (const struct pn_actuator_option *opts,
	   gpointer data)
{
  return;
}

struct pn_actuator_desc builtin_general_noop =
{
  "general_noop", "Do Nothing", "Does absolutely nothing.",
  0, NULL,
  NULL, NULL, general_noop_exec
};

/* **************** general_invert **************** */
static void
general_invert_exec (const struct pn_actuator_option *opts,
	   gpointer data)
{
  int i, j;

  for (j=0; j < pn_image_data->height; j++)
    for (i=0; i < pn_image_data->width; i++)
      pn_image_data->surface[0][PN_IMG_INDEX (i, j)] =
	255 - pn_image_data->surface[0][PN_IMG_INDEX (i, j)];
}

struct pn_actuator_desc builtin_general_invert =
{
  "general_invert", "Value Invert", "Performs a value invert.",
  0, NULL,
  NULL, NULL, general_invert_exec
};

/* **************** general_replace **************** */
static struct pn_actuator_option_desc general_replace_opts[] =
{
  { "start", "The beginning colour value that should be replaced by the value of out.",
    OPT_TYPE_INT, { ival: 250 } },
  { "end", "The ending colour value that should be replaced by the value of out.",
    OPT_TYPE_INT, { ival: 255 } },
  { "out", "The colour value that in is replaced with.",
    OPT_TYPE_INT, { ival: 0 } },
  { NULL }
};

static void
general_replace_exec (const struct pn_actuator_option *opts,
	   gpointer data)
{
  register int i, j;
  register guchar val;
  guchar begin = opts[0].val.ival > 255 || opts[0].val.ival < 0 ? 250 : opts[0].val.ival;
  guchar end = opts[1].val.ival > 255 || opts[1].val.ival < 0 ? 255 : opts[1].val.ival;
  guchar out = opts[2].val.ival > 255 || opts[2].val.ival < 0 ? 0 : opts[2].val.ival;

  for (j=0; j < pn_image_data->height; j++)
    for (i=0; i < pn_image_data->width; i++)
      {
        val = pn_image_data->surface[0][PN_IMG_INDEX (i, j)];
        if (val >= begin && val <= end)
          pn_image_data->surface[0][PN_IMG_INDEX (i, j)] = out;
      }
}

struct pn_actuator_desc builtin_general_replace =
{
  "general_replace", "Value Replace", "Performs a value replace on a range of values.",
  0, general_replace_opts,
  NULL, NULL, general_replace_exec
};

/* **************** general_swap **************** */
static void
general_swap_exec (const struct pn_actuator_option *opts,
	   gpointer data)
{
  pn_swap_surfaces ();
}

struct pn_actuator_desc builtin_general_swap =
{
  "general_swap", "Swap Surface", "Swaps the surface.",
  0, NULL,
  NULL, NULL, general_swap_exec
};
