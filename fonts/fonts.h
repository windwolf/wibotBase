/**
 ******************************************************************************
 * @file    fonts.h
 * @author  MCD Application Team
 * @brief   This file provides definition for fonts to be used for STM32
 *          board's LCD driver
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2014 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "graph.h"

  typedef enum FONT_MEMORY_LAYOUT_DIRECTION
  {
    FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL = 0,
    FONT_MEMORY_LAYOUT_DIRECTION_VERTICAL,
  } FONT_MEMORY_LAYOUT_DIRECTION;

  typedef struct FontInfo
  {
    const uint8_t *table;
    uint16_t width;
    uint16_t height;
    FONT_MEMORY_LAYOUT_DIRECTION direction;
  } FontInfo;

  extern FontInfo Font17x24;

  extern FontInfo Font14x20;

  extern FontInfo Font11x16;

  extern FontInfo Font7x12;

  extern FontInfo Font5x8;

  extern FontInfo Font6x8_v;

  extern FontInfo Font8x16_v;

  typedef struct SpaceInfo
  {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
  } SpaceInfo;

  typedef struct FontDrawInfo
  {
    Color foreColor;
    Color backColor;
    uint8_t spacing;
    uint8_t lineSpacing;
  } FontDrawInfo;

  typedef enum PIXEL_SIZE
  {
    PIXEL_SIZE_1BIT = 0x00000001,
    PIXEL_SIZE_8BIT = 0x000000FF,
    PIXEL_SIZE_16BIT = 0x0000FFFF,
    PIXEL_SIZE_24BIT = 0x00FFFFFF,
    PIXEL_SIZE_32BIT = 0xFFFFFFFF,
  } PIXEL_SIZE;

  typedef enum CANVAS_MEMORY_LAYOUT_DIRECTION
  {
    CANVAS_MEMORY_LAYOUT_DIRECTION_HORIZONTAL = 0,
    CANVAS_MEMORY_LAYOUT_DIRECTION_VERTICAL,
  } CANVAS_MEMORY_LAYOUT_DIRECTION;

  typedef struct CanvasInfo
  {
    uint16_t width;
    uint16_t height;
    PIXEL_SIZE pixelSize;
    CANVAS_MEMORY_LAYOUT_DIRECTION direction;
  } CanvasInfo;

  bool FONTS_CalcSpace(uint16_t x, uint16_t y, char *str, FontInfo *fontInfo, uint8_t spacing, SpaceInfo *spaceInfo);

  bool FONTS_FillData(uint8_t *buffer, CanvasInfo *canvas, uint16_t x, uint16_t y, char *str, FontInfo *fontInfo, FontDrawInfo *fontDrawInfo);

#ifdef __cplusplus
}
#endif

#endif
