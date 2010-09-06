/*
 * Copyright (C) 2006 Evgeniy Stepanov <eugeni.stepanov@gmail.com>
 * Copyright (C) 2010 Grigori Goronzy <greg@geekmind.org>
 *
 * This file is part of libass.
 *
 * libass is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libass is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with libass; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ADM_coreConfig.h"
#include "ass_render.h"

static void ass_reconfigure(ASS_Renderer *priv)
{
    ASS_Settings *settings = &priv->settings;

    priv->render_id++;
    priv->cache.glyph_cache =
        ass_glyph_cache_reset(priv->cache.glyph_cache);
    priv->cache.bitmap_cache =
        ass_bitmap_cache_reset(priv->cache.bitmap_cache);
    priv->cache.composite_cache =
        ass_composite_cache_reset(priv->cache.composite_cache);
    ass_free_images(priv->prev_images_root);
    priv->prev_images_root = 0;

    priv->width = settings->frame_width;
    priv->height = settings->frame_height;
    priv->orig_width = settings->frame_width - settings->left_margin -
        settings->right_margin;
    priv->orig_height = settings->frame_height - settings->top_margin -
        settings->bottom_margin;
    priv->orig_width_nocrop =
        settings->frame_width - FFMAX(settings->left_margin, 0) -
        FFMAX(settings->right_margin, 0);
    priv->orig_height_nocrop =
        settings->frame_height - FFMAX(settings->top_margin, 0) -
        FFMAX(settings->bottom_margin, 0);
}

void ass_set_frame_size(ASS_Renderer *priv, int w, int h)
{
    if (priv->settings.frame_width != w || priv->settings.frame_height != h) {
        priv->settings.frame_width = w;
        priv->settings.frame_height = h;
        if (priv->settings.aspect == 0.) {
            priv->settings.aspect = ((double) w) / h;
            priv->settings.storage_aspect = ((double) w) / h;
        }
        ass_reconfigure(priv);
    }
}

void ass_set_margins(ASS_Renderer *priv, int t, int b, int l, int r)
{
    if (priv->settings.left_margin != l || priv->settings.right_margin != r ||
        priv->settings.top_margin != t || priv->settings.bottom_margin != b) {
        priv->settings.left_margin = l;
        priv->settings.right_margin = r;
        priv->settings.top_margin = t;
        priv->settings.bottom_margin = b;
        ass_reconfigure(priv);
    }
}

void ass_set_use_margins(ASS_Renderer *priv, int use)
{
    priv->settings.use_margins = use;
}

void ass_set_aspect_ratio(ASS_Renderer *priv, double dar, double sar)
{
    if (priv->settings.aspect != dar || priv->settings.storage_aspect != sar) {
        priv->settings.aspect = dar;
        priv->settings.storage_aspect = sar;
        ass_reconfigure(priv);
    }
}

void ass_set_font_scale(ASS_Renderer *priv, double font_scale)
{
    if (priv->settings.font_size_coeff != font_scale) {
        priv->settings.font_size_coeff = font_scale;
        ass_reconfigure(priv);
    }
}

void ass_set_hinting(ASS_Renderer *priv, ASS_Hinting ht)
{
    if (priv->settings.hinting != ht) {
        priv->settings.hinting = ht;
        ass_reconfigure(priv);
    }
}

void ass_set_line_spacing(ASS_Renderer *priv, double line_spacing)
{
    priv->settings.line_spacing = line_spacing;
}

void ass_set_fonts(ASS_Renderer *priv, const char *default_font,
                   const char *default_family, int fc, const char *config,
                   int update)
{
    free(priv->settings.default_font);
    free(priv->settings.default_family);
    priv->settings.default_font = default_font ? strdup(default_font) : 0;
    priv->settings.default_family =
        default_family ? strdup(default_family) : 0;

    if (priv->fontconfig_priv)
        fontconfig_done(priv->fontconfig_priv);
    priv->fontconfig_priv =
        fontconfig_init(priv->library, priv->ftlibrary, default_family,
                        default_font, fc, config, update);
}

int ass_fonts_update(ASS_Renderer *render_priv)
{
    return fontconfig_update(render_priv->fontconfig_priv);
}

void ass_set_cache_limits(ASS_Renderer *render_priv, int glyph_max,
                          int bitmap_max)
{
    render_priv->cache.glyph_max = glyph_max ? glyph_max : GLYPH_CACHE_MAX;
    render_priv->cache.bitmap_max_size = bitmap_max ? 1048576 * bitmap_max :
                                         BITMAP_CACHE_MAX_SIZE;
}
