/*
 * Nestopia UE
 * 
 * Copyright (C) 2012-2014 R. Danbrook
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../main.h"
#include "../config.h"
#include "../audio.h"
#include "../input.h"

#include "gtkui.h"
#include "gtkui_callbacks.h"
#include "gtkui_config.h"

extern settings_t conf;
extern gamepad_t player[NUMGAMEPADS];
extern char padpath[512];

GtkWidget *configwindow;

// Audio
GtkWidget *scale_audio_volume[NUMCHANNELS];
GtkAdjustment *adj_audio_volume[NUMCHANNELS];

// Input
GtkWidget *combo_input_player;
GtkWidget *combo_input_type;
GtkWidget *inputconfbutton;
GtkWidget *entry_input[NUMBUTTONS];

GtkWidget *gtkui_config() {
	// Create the Configuration window
	
	if (configwindow) { return NULL; }
	
	configwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(configwindow), "Configuration");
	
	GtkWidget *box_upper = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *box_lower = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *notebook = gtk_notebook_new();
	
	gtk_container_add(GTK_CONTAINER(configwindow), box_upper);
	
	// Video //
	GtkWidget *box_video = gtk_widget_new(
				GTK_TYPE_BOX,
				"orientation", GTK_ORIENTATION_HORIZONTAL,
				"margin-top", MARGIN_TB,
				"margin-bottom", MARGIN_TB,
				NULL);
	GtkWidget *box_video_l = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *box_video_r = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	
	// Filter
	GtkWidget *box_video_filter = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_video_filter = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "Filter:",
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	GtkWidget *combo_video_filter = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_filter), "None");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_filter), "NTSC");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_filter), "xBR");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_filter), "HqX");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_filter), "2xSaI");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_filter), "ScaleX");
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_video_filter), conf.video_filter);
	
	gtk_box_pack_start(GTK_BOX(box_video_filter), label_video_filter, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_filter), combo_video_filter, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_l), box_video_filter, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_video_filter), "changed",
		G_CALLBACK(gtkui_cb_video_filter), NULL);
	
	// Scale Factor
	GtkWidget *box_video_scale = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_video_scale = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "Scale Factor:",
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	GtkWidget *combo_video_scale = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_scale), "1x");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_scale), "2x");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_scale), "3x");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_scale), "4x");
		
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_video_scale), conf.video_scale_factor - 1);
	
	gtk_box_pack_start(GTK_BOX(box_video_scale), label_video_scale, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_scale), combo_video_scale, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_l), box_video_scale, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_video_scale), "changed",
		G_CALLBACK(gtkui_cb_video_scale), NULL);
	
	// NTSC Mode
	GtkWidget *box_video_ntscmode = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_video_ntscmode = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "NTSC Mode:",
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	GtkWidget *combo_video_ntscmode = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_ntscmode), "Composite");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_ntscmode), "S-Video");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_ntscmode), "RGB");
		
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_video_ntscmode), conf.video_ntsc_mode);
	
	gtk_box_pack_start(GTK_BOX(box_video_ntscmode), label_video_ntscmode, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_ntscmode), combo_video_ntscmode, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_l), box_video_ntscmode, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_video_ntscmode), "changed",
		G_CALLBACK(gtkui_cb_video_ntscmode), NULL);
	
	// xBR Corner Rounding
	GtkWidget *box_video_xbrrounding = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_video_xbrrounding = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "xBR Corner Rounding:",
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	GtkWidget *combo_video_xbrrounding = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_xbrrounding), "None");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_xbrrounding), "Some");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_xbrrounding), "All");
		
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_video_xbrrounding), conf.video_xbr_corner_rounding);
	
	gtk_box_pack_start(GTK_BOX(box_video_xbrrounding), label_video_xbrrounding, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_xbrrounding), combo_video_xbrrounding, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_l), box_video_xbrrounding, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_video_xbrrounding), "changed",
		G_CALLBACK(gtkui_cb_video_xbrrounding), NULL);
	
	// xBR Pixel Blending
	GtkWidget *check_video_xbrpixblend = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "xBR Pixel Blending",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_video_xbrpixblend), conf.video_xbr_pixel_blending);
	
	gtk_box_pack_start(GTK_BOX(box_video_l), check_video_xbrpixblend, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_video_xbrpixblend), "toggled",
		G_CALLBACK(gtkui_cb_video_xbrpixblend), NULL);
	
	// Linear Filter
	GtkWidget *check_video_linear_filter = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Linear Filter",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_video_linear_filter), conf.video_linear_filter);
	
	gtk_box_pack_start(GTK_BOX(box_video_l), check_video_linear_filter, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_video_linear_filter), "toggled",
		G_CALLBACK(gtkui_cb_video_linear_filter), NULL);
	
	// TV Aspect
	GtkWidget *check_video_tv_aspect = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "TV Aspect Ratio",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_video_tv_aspect), conf.video_tv_aspect);
	
	gtk_box_pack_start(GTK_BOX(box_video_l), check_video_tv_aspect, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_video_tv_aspect), "toggled",
		G_CALLBACK(gtkui_cb_video_tv_aspect), NULL);
	
	// Mask Overscan
	GtkWidget *check_video_unmask_overscan = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Unmask Overscan",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_video_unmask_overscan), conf.video_unmask_overscan);
	
	gtk_box_pack_start(GTK_BOX(box_video_l), check_video_unmask_overscan, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_video_unmask_overscan), "toggled",
		G_CALLBACK(gtkui_cb_video_unmask_overscan), NULL);
		
	// Stretch Aspect
	GtkWidget *check_video_stretch_aspect = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Stretch Aspect Ratio",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_video_stretch_aspect), conf.video_stretch_aspect);
	
	gtk_box_pack_start(GTK_BOX(box_video_l), check_video_stretch_aspect, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_video_stretch_aspect), "toggled",
		G_CALLBACK(gtkui_cb_video_stretch_aspect), NULL);
	
	// Unlimited Sprites
	GtkWidget *check_video_unlimited_sprites = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Unlimited Sprites",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_video_unlimited_sprites), conf.video_unlimited_sprites);
	
	gtk_box_pack_start(GTK_BOX(box_video_l), check_video_unlimited_sprites, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_video_unlimited_sprites), "toggled",
		G_CALLBACK(gtkui_cb_video_unlimited_sprites), NULL);
	
	// Palette Mode
	GtkWidget *box_video_palette = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_video_palette = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "Palette Mode:",
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				NULL);
	GtkWidget *combo_video_palette = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_palette), "YUV");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_palette), "RGB");
		
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_video_palette), conf.video_palette_mode);
	
	gtk_box_pack_start(GTK_BOX(box_video_palette), label_video_palette, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_palette), combo_video_palette, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_r), box_video_palette, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_video_palette), "changed",
		G_CALLBACK(gtkui_cb_video_palette), NULL);
	
	// YUV Decoder
	GtkWidget *box_video_decoder = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_video_decoder = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "YUV Decoder:",
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				NULL);
	GtkWidget *combo_video_decoder = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_decoder), "Consumer");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_decoder), "Canonical");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_video_decoder), "Alternative");
		
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_video_decoder), conf.video_decoder);
	
	gtk_box_pack_start(GTK_BOX(box_video_decoder), label_video_decoder, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_decoder), combo_video_decoder, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video_r), box_video_decoder, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_video_decoder), "changed",
		G_CALLBACK(gtkui_cb_video_decoder), NULL);
	
	// Brightness
	GtkAdjustment *adj_video_brightness = gtk_adjustment_new(conf.video_brightness, -100, 100, 1, 5, 0);
	GtkWidget *frame_video_brightness = gtk_frame_new("Brightness");
	GtkWidget *scale_video_brightness = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				"margin-right", MARGIN_LR,
				"orientation", GTK_ORIENTATION_HORIZONTAL,
				"adjustment", adj_video_brightness,
				"width-request", 201,
				"height-request", 32,
				"digits", 0,
				NULL);
	gtk_container_add(GTK_CONTAINER(frame_video_brightness), scale_video_brightness);
	gtk_box_pack_start(GTK_BOX(box_video_r), frame_video_brightness, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(scale_video_brightness), "value-changed",
		G_CALLBACK(gtkui_cb_video_brightness), NULL);
	
	// Saturation
	GtkAdjustment *adj_video_saturation = gtk_adjustment_new(conf.video_saturation, -100, 100, 1, 5, 0);
	GtkWidget *frame_video_saturation = gtk_frame_new("Saturation");
	GtkWidget *scale_video_saturation = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				"margin-right", MARGIN_LR,
				"orientation", GTK_ORIENTATION_HORIZONTAL,
				"adjustment", adj_video_saturation,
				"width-request", 201,
				"height-request", 32,
				"digits", 0,
				NULL);
	gtk_container_add(GTK_CONTAINER(frame_video_saturation), scale_video_saturation);
	gtk_box_pack_start(GTK_BOX(box_video_r), frame_video_saturation, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(scale_video_saturation), "value-changed",
		G_CALLBACK(gtkui_cb_video_saturation), NULL);
	
	// Contrast
	GtkAdjustment *adj_video_contrast = gtk_adjustment_new(conf.video_contrast, -100, 100, 1, 5, 0);
	GtkWidget *frame_video_contrast = gtk_frame_new("Contrast");
	GtkWidget *scale_video_contrast = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				"margin-right", MARGIN_LR,
				"orientation", GTK_ORIENTATION_HORIZONTAL,
				"adjustment", adj_video_contrast,
				"width-request", 201,
				"height-request", 32,
				"digits", 0,
				NULL);
	gtk_container_add(GTK_CONTAINER(frame_video_contrast), scale_video_contrast);
	gtk_box_pack_start(GTK_BOX(box_video_r), frame_video_contrast, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(scale_video_contrast), "value-changed",
		G_CALLBACK(gtkui_cb_video_contrast), NULL);
	
	// Hue
	GtkAdjustment *adj_video_hue = gtk_adjustment_new(conf.video_hue, -45, 45, 1, 5, 0);
	GtkWidget *frame_video_hue = gtk_frame_new("Hue");
	GtkWidget *scale_video_hue = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				"margin-right", MARGIN_LR,
				"orientation", GTK_ORIENTATION_HORIZONTAL,
				"adjustment", adj_video_hue,
				"width-request", 91,
				"height-request", 32,
				"digits", 0,
				NULL);
	gtk_container_add(GTK_CONTAINER(frame_video_hue), scale_video_hue);
	gtk_box_pack_start(GTK_BOX(box_video_r), frame_video_hue, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(scale_video_hue), "value-changed",
		G_CALLBACK(gtkui_cb_video_hue), NULL);
	
	gtk_box_pack_start(GTK_BOX(box_video), box_video_l, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_video), box_video_r, FALSE, FALSE, MARGIN_LR);
	
	// Audio //
	GtkWidget *box_audio = gtk_widget_new(
				GTK_TYPE_BOX,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"margin-top", MARGIN_TB,
				"margin-bottom", MARGIN_TB,
				NULL);
	
	// Audio API
	GtkWidget *box_audio_api = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_audio_api = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "API:",
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	GtkWidget *combo_audio_api = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_audio_api), "SDL");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_audio_api), "libao");
		
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_audio_api), conf.audio_api);
	
	gtk_box_pack_start(GTK_BOX(box_audio_api), label_audio_api, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_audio_api), combo_audio_api, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_audio), box_audio_api, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_audio_api), "changed",
		G_CALLBACK(gtkui_cb_audio_api), NULL);
	
	// Sample Rate
	GtkWidget *box_audio_samplerate = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_audio_samplerate = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "Sample Rate:",
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	GtkWidget *combo_audio_samplerate = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_audio_samplerate), "11025Hz");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_audio_samplerate), "22050Hz");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_audio_samplerate), "44100Hz");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_audio_samplerate), "48000Hz");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_audio_samplerate), "96000Hz");
	
	switch (conf.audio_sample_rate) {
		case 11025:
			gtk_combo_box_set_active(GTK_COMBO_BOX(combo_audio_samplerate), 0);
			break;
		case 22050:
			gtk_combo_box_set_active(GTK_COMBO_BOX(combo_audio_samplerate), 1);
			break;
		case 44100:
			gtk_combo_box_set_active(GTK_COMBO_BOX(combo_audio_samplerate), 2);
			break;
		case 48000:
			gtk_combo_box_set_active(GTK_COMBO_BOX(combo_audio_samplerate), 3);
			break;
		case 96000:
			gtk_combo_box_set_active(GTK_COMBO_BOX(combo_audio_samplerate), 4);
			break;
		default:
			gtk_combo_box_set_active(GTK_COMBO_BOX(combo_audio_samplerate), 2);
			break;
	}
	
	gtk_box_pack_start(GTK_BOX(box_audio_samplerate), label_audio_samplerate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_audio_samplerate), combo_audio_samplerate, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_audio), box_audio_samplerate, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_audio_samplerate), "changed",
		G_CALLBACK(gtkui_cb_audio_samplerate), NULL);
	
	// Stereo
	GtkWidget *check_audio_stereo = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Stereo",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_audio_stereo), conf.audio_stereo);
	
	gtk_box_pack_start(GTK_BOX(box_audio), check_audio_stereo, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_audio_stereo), "toggled",
		G_CALLBACK(gtkui_cb_audio_stereo), NULL);
	
	// Volume
	GtkWidget *label_audio_volume[NUMCHANNELS];
	
	// The Grid
	GtkWidget *grid_audio_volume = gtk_widget_new(
				GTK_TYPE_GRID,
				"column-homogeneous", TRUE,
				"column-spacing", MARGIN_LR,
				"row-spacing", MARGIN_TB,
				"margin", MARGIN_TB,
				NULL);
	
	// Master
	label_audio_volume[0] = gtk_widget_new(GTK_TYPE_LABEL, "label", "Master", NULL);
	adj_audio_volume[0] = gtk_adjustment_new(conf.audio_volume, 0, 100, 1, 5, 0);
	scale_audio_volume[0] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[0],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[0], 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[0], 0, 1, 1, 1);
	
	g_signal_connect(G_OBJECT(scale_audio_volume[0]), "value-changed",
		G_CALLBACK(gtkui_audio_volume_master), NULL);
	
	// Square1
	label_audio_volume[1] = gtk_widget_new(GTK_TYPE_LABEL, "label", "Square1", NULL);
	adj_audio_volume[1] = gtk_adjustment_new(conf.audio_vol_sq1, 0, 100, 1, 5, 0);
	scale_audio_volume[1] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[1],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[1], 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[1], 1, 1, 1, 1);
	
	// Square2
	label_audio_volume[2] = gtk_widget_new(GTK_TYPE_LABEL, "label", "Square2", NULL);
	adj_audio_volume[2] = gtk_adjustment_new(conf.audio_vol_sq2, 0, 100, 1, 5, 0);
	scale_audio_volume[2] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[2],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[2], 2, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[2], 2, 1, 1, 1);
	
	// Triangle
	label_audio_volume[3] = gtk_widget_new(GTK_TYPE_LABEL, "label", "Triangle", NULL);
	adj_audio_volume[3] = gtk_adjustment_new(conf.audio_vol_tri, 0, 100, 1, 5, 0);
	scale_audio_volume[3] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[3],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[3], 3, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[3], 3, 1, 1, 1);
	
	// Noise
	label_audio_volume[4] = gtk_widget_new(GTK_TYPE_LABEL, "label", "Noise", NULL);
	adj_audio_volume[4] = gtk_adjustment_new(conf.audio_vol_noise, 0, 100, 1, 5, 0);
	scale_audio_volume[4] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[4],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[4], 4, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[4], 4, 1, 1, 1);
	
	// Noise
	label_audio_volume[5] = gtk_widget_new(GTK_TYPE_LABEL, "label", "DPCM", NULL);
	adj_audio_volume[5] = gtk_adjustment_new(conf.audio_vol_dpcm, 0, 100, 1, 5, 0);
	scale_audio_volume[5] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[5],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[5], 5, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[5], 5, 1, 1, 1);
	
	// FDS
	label_audio_volume[6] = gtk_widget_new(GTK_TYPE_LABEL, "label", "FDS", NULL);
	adj_audio_volume[6] = gtk_adjustment_new(conf.audio_vol_fds, 0, 100, 1, 5, 0);
	scale_audio_volume[6] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[6],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[6], 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[6], 0, 3, 1, 1);
	
	// MMC5
	label_audio_volume[7] = gtk_widget_new(GTK_TYPE_LABEL, "label", "MMC5", NULL);
	adj_audio_volume[7] = gtk_adjustment_new(conf.audio_vol_mmc5, 0, 100, 1, 5, 0);
	scale_audio_volume[7] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[7],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[7], 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[7], 1, 3, 1, 1);
	
	// VRC6
	label_audio_volume[8] = gtk_widget_new(GTK_TYPE_LABEL, "label", "VRC6", NULL);
	adj_audio_volume[8] = gtk_adjustment_new(conf.audio_vol_vrc6, 0, 100, 1, 5, 0);
	scale_audio_volume[8] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[8],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[8], 2, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[8], 2, 3, 1, 1);
	
	// VRC7
	label_audio_volume[9] = gtk_widget_new(GTK_TYPE_LABEL, "label", "VRC7", NULL);
	adj_audio_volume[9] = gtk_adjustment_new(conf.audio_vol_vrc7, 0, 100, 1, 5, 0);
	scale_audio_volume[9] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[9],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[9], 3, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[9], 3, 3, 1, 1);
	
	// N163
	label_audio_volume[10] = gtk_widget_new(GTK_TYPE_LABEL, "label", "N163", NULL);
	adj_audio_volume[10] = gtk_adjustment_new(conf.audio_vol_n163, 0, 100, 1, 5, 0);
	scale_audio_volume[10] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[10],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[10], 4, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[10], 4, 3, 1, 1);
	
	// S5B
	label_audio_volume[11] = gtk_widget_new(GTK_TYPE_LABEL, "label", "S5B", NULL);
	adj_audio_volume[11] = gtk_adjustment_new(conf.audio_vol_s5b, 0, 100, 1, 5, 0);
	scale_audio_volume[11] = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_CENTER,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"adjustment", adj_audio_volume[11],
				"width-request", 32,
				"height-request", 100,
				"inverted", TRUE,
				"digits", 0,
				NULL);
	
	gtk_grid_attach(GTK_GRID(grid_audio_volume), label_audio_volume[11], 5, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid_audio_volume), scale_audio_volume[11], 5, 3, 1, 1);
	
	// Set the callbacks for every control but master
	for (int i = 1; i < NUMCHANNELS; i++) {
		g_signal_connect(G_OBJECT(scale_audio_volume[i]), "value-changed",
			G_CALLBACK(gtkui_audio_volume), NULL);
	}
	
	// Pack the grid into the box	
	gtk_box_pack_start(GTK_BOX(box_audio), grid_audio_volume, FALSE, FALSE, 0);
	
	// Input //
	GtkWidget *box_input = gtk_widget_new(
				GTK_TYPE_BOX,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"margin-top", MARGIN_TB,
				"margin-bottom", MARGIN_TB,
				NULL);
	GtkWidget *box_input_upper = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *box_input_lower = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	
	gtk_box_pack_start(GTK_BOX(box_input), box_input_upper, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_input), box_input_lower, FALSE, FALSE, 0);
	
	// NES Controller
	GtkWidget *box_input_pad = gtk_widget_new(GTK_TYPE_BOX, "halign", GTK_ALIGN_START, "orientation", GTK_ORIENTATION_VERTICAL, NULL);
	GtkWidget *nespad = gtk_widget_new(
				GTK_TYPE_IMAGE,
				"halign", GTK_ALIGN_CENTER,
				"expand", FALSE,
				"file",	padpath,
				"margin", MARGIN_TB,
				NULL);
	gtk_box_pack_start(GTK_BOX(box_input_pad), nespad, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box_input_upper), box_input_pad, FALSE, FALSE, 0);
	
	// Turbo Pulse
	GtkAdjustment *adj_input_turbopulse = gtk_adjustment_new(conf.timing_turbopulse, 2, 9, 1, 5, 0);
	GtkWidget *box_input_turbopulse = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_input_turbopulse = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "Turbo Pulse",
				"halign", GTK_ALIGN_START,
				"margin-top", MARGIN_TB,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				"margin-right", MARGIN_LR,
				NULL);
	GtkWidget *scale_input_turbopulse = gtk_widget_new(
				GTK_TYPE_SCALE,
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				"orientation", GTK_ORIENTATION_HORIZONTAL,
				"adjustment", adj_input_turbopulse,
				"width-request", 60,
				"height-request", 32,
				"digits", 0,
				NULL);
	gtk_box_pack_start(GTK_BOX(box_input_turbopulse), label_input_turbopulse, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_input_turbopulse), scale_input_turbopulse, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_input_pad), box_input_turbopulse, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(scale_input_turbopulse), "value-changed",
		G_CALLBACK(gtkui_cb_input_turbopulse), NULL);
	
	// Options Box
	GtkWidget *box_input_options = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(box_input_upper), box_input_options, FALSE, FALSE, 0);
	
	GtkWidget *inputsep1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_box_pack_start(GTK_BOX(box_input_lower), inputsep1, FALSE, FALSE, 0);
	
	// Player Select
	combo_input_player = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-top", MARGIN_TB,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR * 2,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combo_input_player), "Player 1");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combo_input_player), "Player 2");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_input_player), 0);
	gtk_box_pack_start(GTK_BOX(box_input_options), combo_input_player, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_input_player), "changed",
		G_CALLBACK(gtkui_config_input_refresh), NULL);
	
	// Device Type Select
	combo_input_type = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-top", MARGIN_TB,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR * 2,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combo_input_type), "Keyboard");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combo_input_type), "Joysick");
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_input_type), 0);
	gtk_box_pack_start(GTK_BOX(box_input_options), combo_input_type, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_input_type), "changed",
		G_CALLBACK(gtkui_config_input_refresh), NULL);
	
	// The Input Config button
	inputconfbutton = gtk_widget_new(
				GTK_TYPE_BUTTON,
				"label", "Configure",
				"halign", GTK_ALIGN_START,
				"margin-top", MARGIN_TB,
				"margin-left", MARGIN_LR * 2,
				NULL);
	gtk_box_pack_start(GTK_BOX(box_input_options), inputconfbutton, FALSE, FALSE, 0);
	
	// Connect the button to a callback
	g_signal_connect(G_OBJECT(inputconfbutton), "clicked",
		G_CALLBACK(gtkui_config_input), NULL);
	
	// Key translation only needs to be done for release events
	g_signal_connect(G_OBJECT(configwindow), "key-release-event",
		G_CALLBACK(gtkui_cb_convert_key), NULL);
	
	// The Grid
	GtkWidget *grid_input_lower = gtk_widget_new(
				GTK_TYPE_GRID,
				"column-spacing", MARGIN_LR,
				"row-spacing", MARGIN_TB,
				"margin", MARGIN_TB,
				NULL);
	
	GtkWidget *label_input_up = gtk_widget_new(GTK_TYPE_LABEL, "label", "Up:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_up, 0, 0, 1, 1);
	
	GtkWidget *label_input_down = gtk_widget_new(GTK_TYPE_LABEL, "label", "Down:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_down, 0, 1, 1, 1);
	
	GtkWidget *label_input_left = gtk_widget_new(GTK_TYPE_LABEL, "label", "Left:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_left, 0, 2, 1, 1);
	
	GtkWidget *label_input_right = gtk_widget_new(GTK_TYPE_LABEL, "label", "Right:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_right, 0, 3, 1, 1);
	
	GtkWidget *label_input_select = gtk_widget_new(GTK_TYPE_LABEL, "label", "Select:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_select, 0, 4, 1, 1);
	
	entry_input[0] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[0]), SDL_GetScancodeName(player[0].u));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[0], 1, 0, 1, 1);
	
	entry_input[1] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[1]), SDL_GetScancodeName(player[0].d));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[1], 1, 1, 1, 1);
	
	entry_input[2] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[2]), SDL_GetScancodeName(player[0].l));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[2], 1, 2, 1, 1);
	
	entry_input[3] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[3]), SDL_GetScancodeName(player[0].r));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[3], 1, 3, 1, 1);
	
	entry_input[4] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[4]), SDL_GetScancodeName(player[0].select));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[4], 1, 4, 1, 1);
	
	GtkWidget *label_input_start = gtk_widget_new(GTK_TYPE_LABEL, "label", "Start:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_start, 2, 0, 1, 1);
	
	GtkWidget *label_input_a = gtk_widget_new(GTK_TYPE_LABEL, "label", "A:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_a, 2, 1, 1, 1);
	
	GtkWidget *label_input_b = gtk_widget_new(GTK_TYPE_LABEL, "label", "B:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_b, 2, 2, 1, 1);
	
	GtkWidget *label_input_ta = gtk_widget_new(GTK_TYPE_LABEL, "label", "Turbo A:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_ta, 2, 3, 1, 1);
	
	GtkWidget *label_input_tb = gtk_widget_new(GTK_TYPE_LABEL, "label", "Turbo B:", "halign", GTK_ALIGN_START, NULL);
	gtk_grid_attach(GTK_GRID(grid_input_lower), label_input_tb, 2, 4, 1, 1);
	
	entry_input[5] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[5]), SDL_GetScancodeName(player[0].start));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[5], 3, 0, 1, 1);
	
	entry_input[6] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[6]), SDL_GetScancodeName(player[0].a));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[6], 3, 1, 1, 1);
	
	entry_input[7] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[7]), SDL_GetScancodeName(player[0].b));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[7], 3, 2, 1, 1);
	
	entry_input[8] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[8]), SDL_GetScancodeName(player[0].ta));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[8], 3, 3, 1, 1);
	
	entry_input[9] = gtk_widget_new(GTK_TYPE_ENTRY, FALSE, NULL);
	gtk_entry_set_text(GTK_ENTRY(entry_input[9]), SDL_GetScancodeName(player[0].tb));
	gtk_grid_attach(GTK_GRID(grid_input_lower), entry_input[9], 3, 4, 1, 1);
	
	gtk_box_pack_start(GTK_BOX(box_input_lower), grid_input_lower, FALSE, FALSE, 0);
	
	// Misc //
	GtkWidget *box_misc = gtk_widget_new(
				GTK_TYPE_BOX,
				"orientation", GTK_ORIENTATION_VERTICAL,
				"margin-top", MARGIN_TB,
				"margin-bottom", MARGIN_TB,
				NULL);
	
	// Default System
	GtkWidget *box_misc_default_system = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *label_misc_default_system = gtk_widget_new(
				GTK_TYPE_LABEL,
				"label", "Default System:",
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	GtkWidget *combo_misc_default_system = gtk_widget_new(
				GTK_TYPE_COMBO_BOX_TEXT,
				"halign", GTK_ALIGN_START,
				"margin-bottom", MARGIN_TB,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_misc_default_system), "NTSC");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_misc_default_system), "PAL");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_misc_default_system), "Famicom");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_misc_default_system), "Dendy");
		
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_misc_default_system), conf.misc_default_system);
	
	gtk_box_pack_start(GTK_BOX(box_misc_default_system), label_misc_default_system, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_misc_default_system), combo_misc_default_system, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_misc), box_misc_default_system, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(combo_misc_default_system), "changed",
		G_CALLBACK(gtkui_cb_misc_default_system), NULL);
	
	// Vsync
	GtkWidget *check_timing_vsync = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Vsync",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_timing_vsync), conf.timing_vsync);
	
	gtk_box_pack_start(GTK_BOX(box_misc), check_timing_vsync, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_timing_vsync), "toggled",
		G_CALLBACK(gtkui_cb_timing_vsync), NULL);
	
	// Limiter
	GtkWidget *check_timing_limiter = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Speed Limiter",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_timing_limiter), conf.timing_limiter);
	
	gtk_box_pack_start(GTK_BOX(box_misc), check_timing_limiter, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_timing_limiter), "toggled",
		G_CALLBACK(gtkui_cb_timing_limiter), NULL);
	
	// Soft Patching
	GtkWidget *check_misc_soft_patching = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Automatic Soft Patching",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_misc_soft_patching), conf.misc_soft_patching);
	
	gtk_box_pack_start(GTK_BOX(box_misc), check_misc_soft_patching, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_misc_soft_patching), "toggled",
		G_CALLBACK(gtkui_cb_misc_soft_patching), NULL);
	
	// Game Genie Sound Distortion
	GtkWidget *check_misc_genie_distortion = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Game Genie Sound Distortion",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_misc_genie_distortion), conf.misc_genie_distortion);
	
	gtk_box_pack_start(GTK_BOX(box_misc), check_misc_genie_distortion, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_misc_genie_distortion), "toggled",
		G_CALLBACK(gtkui_cb_misc_genie_distortion), NULL);
	
	// Disable GUI
	GtkWidget *check_misc_disable_gui = gtk_widget_new(
				GTK_TYPE_CHECK_BUTTON,
				"label", "Disable GUI",
				"halign", GTK_ALIGN_START,
				"margin-left", MARGIN_LR,
				NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_misc_disable_gui), conf.misc_disable_gui);
	
	//gtk_box_pack_start(GTK_BOX(box_misc), check_misc_disable_gui, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(check_misc_disable_gui), "toggled",
		G_CALLBACK(gtkui_cb_misc_disable_gui), NULL);
	
	// Structuring the notebook
	GtkWidget *label_video = gtk_label_new("Video");
	GtkWidget *label_audio = gtk_label_new("Audio");
	GtkWidget *label_input = gtk_label_new("Input");
	GtkWidget *label_misc = gtk_label_new("Misc");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box_video, label_video);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box_audio, label_audio);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box_input, label_input);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box_misc, label_misc);
	
	// The OK button
	GtkWidget *okbutton = gtk_widget_new(
				GTK_TYPE_BUTTON,
				"label", GTK_STOCK_OK,
				"halign", GTK_ALIGN_END,
				"margin-top", 8,
				"margin-bottom", 8,
				"margin-right", 8,
				NULL);
	gtk_button_set_use_stock(GTK_BUTTON(okbutton), TRUE);
	
	// Connect the OK button to a callback
	g_signal_connect(G_OBJECT(okbutton), "clicked",
		G_CALLBACK(gtkui_config_ok), NULL);
	
	// Structuring the window
	gtk_box_pack_start(GTK_BOX(box_upper), notebook, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box_upper), box_lower, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_lower), okbutton, FALSE, FALSE, 0);
	
	g_signal_connect(G_OBJECT(configwindow), "destroy",
		G_CALLBACK(gtkui_config_ok), NULL);
	
	gtk_widget_show_all(configwindow);
	
	return configwindow;
}

void gtkui_config_ok() {
	gtk_widget_destroy(configwindow);
	configwindow = NULL;
}

void gtkui_audio_volume() {
	// Set the audio volume on specific channels
	conf.audio_vol_sq1 = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[1]);
	conf.audio_vol_sq2 = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[2]);
	conf.audio_vol_tri = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[3]);
	conf.audio_vol_noise = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[4]);
	conf.audio_vol_dpcm = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[5]);
	conf.audio_vol_fds = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[6]);
	conf.audio_vol_mmc5 = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[7]);
	conf.audio_vol_vrc6 = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[8]);
	conf.audio_vol_vrc7 = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[9]);
	conf.audio_vol_n163 = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[10]);
	conf.audio_vol_s5b = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[11]);
	audio_adj_volume();
}

void gtkui_audio_volume_master() {
	// Set the audio volume on all channels
	conf.audio_volume = (int)gtk_range_get_value((GtkRange*)scale_audio_volume[0]);
	
	for (int i = 1; i < NUMCHANNELS; i++) {
		gtk_adjustment_set_value(adj_audio_volume[i], gtk_range_get_value((GtkRange*)scale_audio_volume[0]));
	}
	
	gtkui_audio_volume();
}

void gtkui_config_input() {
	// Start the input configuration routine
	gtk_widget_grab_focus(entry_input[0]);
	int pnum = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_input_player));
	int type = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_input_type));
	input_configure(pnum, type);
}

void gtkui_config_input_refresh() {
	// Refresh the input fields
	int pnum = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_input_player));
	int type = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_input_type));
	gtkui_config_input_fields(type, pnum);
}

void gtkui_config_input_fields(int type, int pnum) {
	// Set the text in the input fields based on the current settings
	if (type == 0) {
		gtk_entry_set_text(GTK_ENTRY(entry_input[0]), SDL_GetScancodeName(player[pnum].u));
		gtk_entry_set_text(GTK_ENTRY(entry_input[1]), SDL_GetScancodeName(player[pnum].d));
		gtk_entry_set_text(GTK_ENTRY(entry_input[2]), SDL_GetScancodeName(player[pnum].l));
		gtk_entry_set_text(GTK_ENTRY(entry_input[3]), SDL_GetScancodeName(player[pnum].r));
		gtk_entry_set_text(GTK_ENTRY(entry_input[4]), SDL_GetScancodeName(player[pnum].select));
		gtk_entry_set_text(GTK_ENTRY(entry_input[5]), SDL_GetScancodeName(player[pnum].start));
		gtk_entry_set_text(GTK_ENTRY(entry_input[6]), SDL_GetScancodeName(player[pnum].a));
		gtk_entry_set_text(GTK_ENTRY(entry_input[7]), SDL_GetScancodeName(player[pnum].b));
		gtk_entry_set_text(GTK_ENTRY(entry_input[8]), SDL_GetScancodeName(player[pnum].ta));
		gtk_entry_set_text(GTK_ENTRY(entry_input[9]), SDL_GetScancodeName(player[pnum].tb));
	}
	if (type == 1) {
		gtk_entry_set_text(GTK_ENTRY(entry_input[0]), input_translate_event(player[pnum].ju));
		gtk_entry_set_text(GTK_ENTRY(entry_input[1]), input_translate_event(player[pnum].jd));
		gtk_entry_set_text(GTK_ENTRY(entry_input[2]), input_translate_event(player[pnum].jl));
		gtk_entry_set_text(GTK_ENTRY(entry_input[3]), input_translate_event(player[pnum].jr));
		gtk_entry_set_text(GTK_ENTRY(entry_input[4]), input_translate_event(player[pnum].jselect));
		gtk_entry_set_text(GTK_ENTRY(entry_input[5]), input_translate_event(player[pnum].jstart));
		gtk_entry_set_text(GTK_ENTRY(entry_input[6]), input_translate_event(player[pnum].ja));
		gtk_entry_set_text(GTK_ENTRY(entry_input[7]), input_translate_event(player[pnum].jb));
		gtk_entry_set_text(GTK_ENTRY(entry_input[8]), input_translate_event(player[pnum].jta));
		gtk_entry_set_text(GTK_ENTRY(entry_input[9]), input_translate_event(player[pnum].jtb));
	}
}

void gtkui_config_input_focus(int counter) {
	// Set focus on the proper input field
	switch(counter) {
		case 0: gtk_widget_grab_focus(entry_input[1]); break;
		case 1: gtk_widget_grab_focus(entry_input[2]); break;
		case 2: gtk_widget_grab_focus(entry_input[3]); break;
		case 3: gtk_widget_grab_focus(entry_input[4]); break;
		case 4: gtk_widget_grab_focus(entry_input[5]); break;
		case 5: gtk_widget_grab_focus(entry_input[6]); break;
		case 6: gtk_widget_grab_focus(entry_input[7]); break;
		case 7: gtk_widget_grab_focus(entry_input[8]); break;
		case 8: gtk_widget_grab_focus(entry_input[9]); break;
		default: gtk_widget_grab_focus(entry_input[0]); break;
	}
}