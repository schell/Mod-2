/*
 *  SSGL.h
 *  Mod-2
 *
 *  Created by Schell Scivally on 2/21/11.
 *  Copyright 2011 Electrunique. All rights reserved.
 *
 */

#ifndef __SSGL__
#define __SSGL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAO 6.28318531
#define ONE_DEGREE 0.0174532925
#define ASCII_START 0x21
#define ASCII_END 0x7E

#pragma mark -
#pragma mark Enums

// uniform index
typedef enum {
    SSUniformSampler,
	SSUniformProjection,
	SSUniformModelView,
    SSUniformCount
} SSUniform;

// attribute index
typedef enum {
    SSAttributeVertex,
    SSAttributeTexCoord,
    SSAttributeCount
} SSAttribute;

#pragma mark -
#pragma mark Data Types

typedef float mat4[16];		// 4x4 matrix
typedef float vec3[3];		// 3 component vector
typedef float vec2[2];		// 2 component vector

struct geomap {				// geometry mapping structure 
	float* vertices;		
	float* uvs;
	unsigned int numfloats;
};
typedef struct geomap geomap_;

struct sprite {
	mat4 matrix;			// transformation matrix
	vec2 extants;			// halfwidth and halfheight of sprite
	unsigned int texture;	// texture id
};
typedef struct sprite sprite;

struct spritesheet {		// a sheet containing sprite animations
	sprite sprite;
	float framewidth;		
	float frameheight;
	unsigned int width;		// the number of frames wide
	unsigned int height;	// the number of frames tall
};
typedef struct spritesheet spritesheet;

struct subspritesheet {		// a sheet made from a subset of another sheet
	spritesheet spritesheet;
	unsigned int width;
	unsigned int height;
	unsigned int xoffset;
	unsigned int yoffset;
};
typedef struct subspritesheet subspritesheet;

#pragma mark -
#pragma mark Vector Ops

void vec3Print(vec3* vec);

#pragma mark -
#pragma mark Matrix Ops

void mat4LoadMatrix(mat4* into, mat4* from);
void mat4LoadIdentity(mat4* matrix);
void mat4LoadOrtho(mat4* matrix, float left, float right, float top, float bottom, float far, float near);
void mat4Multiply(mat4* matrixInto, mat4* matrixBy);	
void mat4Frustum(mat4* matrix, float left, float right, float bottom, float top, float nearZ, float farZ);	
void mat4Perspective(mat4* matrix, float fovy, float aspect, float nearZ, float farZ);	
void mat4Scale(mat4* matrix, float sx, float sy, float sz);
void mat4Translate(mat4* matrix, float tx, float ty, float tz);
void mat4MoveForward(mat4* matrix, float amount);
void mat4MoveBackward(mat4* matrix, float amount);
void mat4MoveDown(mat4* matrix, float amount);
void mat4MoveUp(mat4* matrix, float amount); 
void mat4MoveLeft(mat4* matrix, float amount);
void mat4MoveRight(mat4* matrix, float amount); 
void mat4Rotate(mat4* matrix, float angle, float x, float y, float z);	
void mat4SpinRight(mat4* matrix, float amount);
void mat4SpinLeft(mat4* matrix, float amount);
void mat4SpinDown(mat4* matrix, float amount);
void mat4SpinUp(mat4* matrix, float amount);
void mat4SpinClockwise(mat4* matrix, float amount);
void mat4SpinCounterClockwise(mat4* matrix, float amount);
void mat4Print(mat4* matrix);
void mat4Right(mat4* matrix, vec3* vec);
void mat4Up(mat4* matrix, vec3* vec);
void mat4Out(mat4* matrix, vec3* vec);

#pragma mark -
#pragma mark Sprite Ops

sprite spriteMake(float halfwidth, float halfheight, unsigned int texture);
void spritePrint(sprite s);
void spriteTile(sprite* s);

spritesheet spritesheetMake(sprite s, float framewidth, float frameheight, int width, int height);

#pragma mark -
#pragma mark Geometry Mapping

geomap_ geomapMake(sprite s);
geomap_ geomapMakeTiled(sprite s, float width, float height);
geomap_ geomapMakeFromSpriteSheet(spritesheet sheet, unsigned int x, unsigned int y);
geomap_ geomapMakeFromSubSpriteSheet(subspritesheet subsheet, unsigned int x, unsigned int y);
void geomapDestroy(geomap_* map);

#pragma mark -
#pragma mark Convenient Destruction

void destroy(geomap_* map);

#endif