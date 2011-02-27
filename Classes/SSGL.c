/*
 *  SSGL.c
 *  Mod-2
 *
 *  Created by Schell Scivally on 2/21/11.
 *  Copyright 2011 Electrunique. All rights reserved.
 *
 */

#include "SSGL.h"

#pragma mark -
#pragma mark Vector Ops

void vec3Print(vec3* vec) {
	printf("vector:\n\t%f %f %f\n",(*vec)[0],(*vec)[1],(*vec)[2]);
}

#pragma mark -
#pragma mark Matrix Ops

// load the a matrix
void mat4LoadMatrix(mat4* into, mat4* from) {
	memcpy(into, from, sizeof(mat4));
}

// load the identity matrix
void mat4LoadIdentity(mat4* matrix) {
	(*matrix)[0] = 1.0f;
	(*matrix)[1] = 0.0f;
	(*matrix)[2] = 0.0f;
	(*matrix)[3] = 0.0f;

	(*matrix)[4] = 0.0f;
	(*matrix)[5] = 1.0f;
	(*matrix)[6] = 0.0f;
	(*matrix)[7] = 0.0f;
	
	(*matrix)[8] = 0.0f;
	(*matrix)[9] = 0.0f;
	(*matrix)[10] = 1.0f;
	(*matrix)[11] = 0.0f;
	
	(*matrix)[12] = 0.0f;
	(*matrix)[13] = 0.0f;
	(*matrix)[14] = 0.0f;
	(*matrix)[15] = 1.0f;
}

// load a parallel projection matrix
void mat4LoadOrtho(mat4* matrix, float left, float right, float top, float bottom, float far, float near) {
	float a,b,c,x,y,z;
	a = 2.0f/(right-left);
	b = 2.0f/(top-bottom);
	c = -2.0f/(far-near);
	x = -(right+left)/(right-left);
	y = -(top+bottom)/(top-bottom);
	z = -(far+near)/(far-near);
	mat4 mat = {  a ,0.0f,0.0f,  x ,
				0.0f,  b ,0.0f,  y ,
				0.0f,0.0f,  c ,  z ,
				0.0f,0.0f,0.0f,1.0f};
	mat4LoadMatrix(matrix, &mat);
}

void mat4Print(mat4* matrix) {
	printf("matrix:\n\t%f %f %f %f\n\t%f %f %f %f\n\t%f %f %f %f\n\t%f %f %f %f\n",
		   (*matrix)[0],(*matrix)[1],(*matrix)[2],(*matrix)[3],
		   (*matrix)[4],(*matrix)[5],(*matrix)[6],(*matrix)[7],
		   (*matrix)[8],(*matrix)[9],(*matrix)[10],(*matrix)[11],
		   (*matrix)[12],(*matrix)[13],(*matrix)[14],(*matrix)[15]);
}

void mat4Multiply(mat4* matrixInto, mat4* matrixBy) {
	mat4 tmpels; 
	memcpy(&tmpels, matrixInto, sizeof(mat4));
	for (int i = 0; i<4; i++)
	{
		tmpels[i*4+0] = ((*matrixInto)[i*4+0] * (*matrixBy)[0*4+0]) +
		((*matrixInto)[i*4+1] * (*matrixBy)[1*4+0]) +
		((*matrixInto)[i*4+2] * (*matrixBy)[2*4+0]) +
		((*matrixInto)[i*4+3] * (*matrixBy)[3*4+0]);
		
		tmpels[i*4+1] = ((*matrixInto)[i*4+0] * (*matrixBy)[0*4+1]) + 
		((*matrixInto)[i*4+1] * (*matrixBy)[1*4+1]) +
		((*matrixInto)[i*4+2] * (*matrixBy)[2*4+1]) +
		((*matrixInto)[i*4+3] * (*matrixBy)[3*4+1]);
		
		tmpels[i*4+2] = ((*matrixInto)[i*4+0] * (*matrixBy)[0*4+2]) + 
		((*matrixInto)[i*4+1] * (*matrixBy)[1*4+2]) +
		((*matrixInto)[i*4+2] * (*matrixBy)[2*4+2]) +
		((*matrixInto)[i*4+3] * (*matrixBy)[3*4+2]);
		
		tmpels[i*4+3] = ((*matrixInto)[i*4+0] * (*matrixBy)[0*4+3]) + 
		((*matrixInto)[i*4+1] * (*matrixBy)[1*4+3]) +
		((*matrixInto)[i*4+2] * (*matrixBy)[2*4+3]) +
		((*matrixInto)[i*4+3] * (*matrixBy)[3*4+3]);
	}
	memcpy(matrixInto, &tmpels, sizeof(mat4));
}

void mat4Frustum(mat4* matrix, float left, float right, float bottom, float top, float nearZ, float farZ) {
	float       deltaX = right - left;
	float       deltaY = top - bottom;
	float       deltaZ = farZ - nearZ;
	if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
		(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) ) {
		return;
	}
	
	mat4LoadIdentity(matrix);
	
	(*matrix)[0*4+0] = 2.0f * nearZ / deltaX;
	(*matrix)[0*4+1] = (*matrix)[0*4+2] = (*matrix)[0*4+3] = 0.0f;
	
	(*matrix)[1*4+1] = 2.0f * nearZ / deltaY;
	(*matrix)[1*4+0] = (*matrix)[1*4+2] = (*matrix)[1*4+3] = 0.0f;
	
	(*matrix)[2*4+0] = (right + left) / deltaX;
	(*matrix)[2*4+1] = (top + bottom) / deltaY;
	(*matrix)[2*4+2] = -(nearZ + farZ) / deltaZ;
	(*matrix)[2*4+3] = -1.0f;
	
	(*matrix)[3*4+2] = -2.0f * nearZ * farZ / deltaZ;
	(*matrix)[3*4+0] = (*matrix)[3*4+1] = (*matrix)[3*4+3] = 0.0f;
}

void mat4Perspective(mat4* matrix, float fovy, float aspect, float nearZ, float farZ) {
	float frustumW, frustumH;
	
	frustumH = tanf( fovy / 360.0f * TAO/2 ) * nearZ;
	frustumW = frustumH * aspect;
	mat4Frustum(matrix, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
}

void mat4Scale(mat4* matrix, float sx, float sy, float sz) {
	(*matrix)[0*4+0] *= sx;
	(*matrix)[0*4+1] *= sx;
	(*matrix)[0*4+2] *= sx;
	(*matrix)[0*4+3] *= sx;
	
	(*matrix)[1*4+0] *= sy;
	(*matrix)[1*4+1] *= sy;
	(*matrix)[1*4+2] *= sy;
	(*matrix)[1*4+3] *= sy;
	
	(*matrix)[2*4+0] *= sz;
	(*matrix)[2*4+1] *= sz;
	(*matrix)[2*4+2] *= sz;
	(*matrix)[2*4+3] *= sz;
}

void mat4Translate(mat4* matrix, float tx, float ty, float tz) {
    (*matrix)[3*4+0] += ((*matrix)[0*4+0] * tx + (*matrix)[1*4+0] * ty + (*matrix)[2*4+0] * tz);
    (*matrix)[3*4+1] += ((*matrix)[0*4+1] * tx + (*matrix)[1*4+1] * ty + (*matrix)[2*4+1] * tz);
    (*matrix)[3*4+2] += ((*matrix)[0*4+2] * tx + (*matrix)[1*4+2] * ty + (*matrix)[2*4+2] * tz);
    (*matrix)[3*4+3] += ((*matrix)[0*4+3] * tx + (*matrix)[1*4+3] * ty + (*matrix)[2*4+3] * tz);
}

void mat4Right(mat4* matrix, vec3* vec) {
	(*vec)[0] = (*matrix)[0];
	(*vec)[1] = (*matrix)[4];
	(*vec)[2] = (*matrix)[8];
}

void mat4Up(mat4* matrix, vec3* vec) {
	(*vec)[0] = (*matrix)[1];
	(*vec)[1] = (*matrix)[5];
	(*vec)[2] = (*matrix)[9];
}

void mat4Out(mat4* matrix, vec3* vec) {
	(*vec)[0] = (*matrix)[2];
	(*vec)[1] = (*matrix)[6];
	(*vec)[2] = (*matrix)[10];
}

void mat4MoveForward(mat4* matrix, float amount) {
	vec3 u = {0.0f,0.0f,0.0f};
	mat4Out(matrix, &u);
	mat4Translate(matrix, u[0]*amount, u[1]*amount, u[2]*amount);
}

void mat4MoveBackward(mat4* matrix, float amount) {
	mat4MoveForward(matrix, -amount);
}

void mat4MoveDown(mat4* matrix, float amount) {
	vec3 u = {0.0f,0.0f,0.0f};
	mat4Up(matrix, &u);
	mat4Translate(matrix, u[0]*amount, u[1]*amount, u[2]*amount);
}

void mat4MoveUp(mat4* matrix, float amount) {
	mat4MoveDown(matrix, -amount);
}

void mat4MoveLeft(mat4* matrix, float amount) {
	vec3 u = {0.0f,0.0f,0.0f};
	mat4Right(matrix, &u);
	mat4Translate(matrix, u[0]*amount, u[1]*amount, u[2]*amount);
}

void mat4MoveRight(mat4* matrix, float amount) {
	mat4MoveLeft(matrix, -amount);
}

void mat4Rotate(mat4* matrix, float angle, float x, float y, float z) {
	float sinAngle, cosAngle;
	float mag = sqrtf(x * x + y * y + z * z);
	
	sinAngle = sinf ( angle * ONE_DEGREE );
	cosAngle = cosf ( angle * ONE_DEGREE );
	if ( mag > 0.0f )
	{
		float xx, yy, zz, xy, yz, zx, xs, ys, zs;
		float oneMinusCos;
		
		x /= mag;
		y /= mag;
		z /= mag;
		
		xx = x * x;
		yy = y * y;
		zz = z * z;
		xy = x * y;
		yz = y * z;
		zx = z * x;
		xs = x * sinAngle;
		ys = y * sinAngle;
		zs = z * sinAngle;
		oneMinusCos = 1.0f - cosAngle;
		
		mat4 rotationMatrix;
		mat4LoadIdentity(&rotationMatrix);
		
		rotationMatrix[0*4+0] = (oneMinusCos * xx) + cosAngle;
		rotationMatrix[0*4+1] = (oneMinusCos * xy) + zs;
		rotationMatrix[0*4+2] = (oneMinusCos * zx) - ys;
		rotationMatrix[0*4+3] = 0.0F;
		
		rotationMatrix[1*4+0] = (oneMinusCos * xy) - zs;
		rotationMatrix[1*4+1] = (oneMinusCos * yy) + cosAngle;
		rotationMatrix[1*4+2] = (oneMinusCos * yz) + xs;
		rotationMatrix[1*4+3] = 0.0F;
		
		rotationMatrix[2*4+0] = (oneMinusCos * zx) + ys;
		rotationMatrix[2*4+1] = (oneMinusCos * yz) - xs;
		rotationMatrix[2*4+2] = (oneMinusCos * zz) + cosAngle;
		rotationMatrix[2*4+3] = 0.0F;
		
		rotationMatrix[3*4+0] = 0.0F; 
		rotationMatrix[3*4+1] = 0.0F;
		rotationMatrix[3*4+2] = 0.0F; 
		rotationMatrix[3*4+3] = 1.0F;
		
		mat4Multiply(matrix, &rotationMatrix);
	}
}

void mat4SpinRight(mat4* matrix, float amount) {
	if (!amount) {
		amount = ONE_DEGREE;
	}
	mat4Rotate(matrix, amount, 0.0, 1.0, 0.0);
}

void mat4SpinLeft(mat4* matrix, float amount) {
	mat4SpinRight(matrix, -amount);
}

void mat4SpinDown(mat4* matrix, float amount) {
	if (!amount) {
		amount = ONE_DEGREE;
	}
	mat4Rotate(matrix, amount, 1.0, 0.0, 0.0);
}

void mat4SpinUp(mat4* matrix, float amount) {
	mat4SpinDown(matrix, -amount);
}

void mat4SpinClockwise(mat4* matrix, float amount) {
	if (!amount) {
		amount = ONE_DEGREE;
	}
	mat4Rotate(matrix, amount, 0.0, 0.0, 1.0);
}

void mat4SpinCounterClockwise(mat4* matrix, float amount) {
	mat4SpinClockwise(matrix, -amount);
}

#pragma mark -
#pragma mark Sprite Ops

sprite spriteMake(float width, float height, unsigned int texture) {
	sprite s;
	mat4LoadIdentity(&s.matrix);
	s.extants[0] = width/2;
	s.extants[1] = height/2;
	s.texture = texture;
	return s;
}

void spritePrint(sprite s) {
	printf("sprite:\n\twidth:%f\n\theight:%f\n\ttexture id:%i\n",s.extants[0]*2.0,s.extants[1]*2.0,s.texture);
}

spritesheet spritesheetMake(sprite s, float framewidth, float frameheight, int width, int height) {
	spritesheet ss;
	ss.sprite = s;
	ss.framewidth = framewidth;
	ss.frameheight = frameheight;
	ss.width = width;
	ss.height = height;
	return ss;
}

#pragma mark -
#pragma mark Geometry Mapping

geomap_ geomapMake(sprite s) {
	geomap_ map;
	size_t size = 8 * sizeof(float);
	map.numfloats = 8;
	map.vertices = malloc(size);
	map.uvs = malloc(size);
	
	float vertices[] = {
        -s.extants[0], -s.extants[1],
		 s.extants[0], -s.extants[1],
        -s.extants[0],  s.extants[1],
		 s.extants[0],  s.extants[1]
    };
	
	static const float uvs[] = {
		1, 1,
		0.0, 1,
		1, 0.0,
		0.0, 0.0
    };
	
	memcpy(map.vertices, vertices, size);
	memcpy(map.uvs, uvs, size);
	return map;
}

geomap_ geomapMakeTiled(sprite s, float width, float height) {
	geomap_ map = geomapMake(s);
	for (int i = 0; i < map.numfloats; i+=2) {
		map.vertices[i] = map.vertices[i]/s.extants[0] * width/2.0;
		map.vertices[i+1] = map.vertices[i+1]/s.extants[1] * height/2.0;
		map.uvs[i] *= width/2.0/s.extants[0];
		map.uvs[i+1] *= height/2.0/s.extants[1];
	}
	return map;
}

geomap_ geomapMakeFromSpriteSheet(spritesheet sheet, unsigned int x, unsigned int y) {
	sprite s = sheet.sprite;
	s.extants[0] = sheet.framewidth/2.0;
	s.extants[1] = sheet.frameheight/2.0;
	geomap_ map = geomapMake(s);
	float px, py;
	px = 1.0/((sheet.sprite.extants[0]*2.0)/sheet.framewidth);
	py = 1.0/((sheet.sprite.extants[0]*2.0)/sheet.frameheight);
	vec2 lowerleft;
	lowerleft[0] = x * px;
	lowerleft[1] = y * py;
	float uvs[] = {
		lowerleft[0],   lowerleft[1],		// lower left
		lowerleft[0]+px,lowerleft[1],		// lower right
		lowerleft[0],   lowerleft[1]+py,	// upper left
		lowerleft[0]+px,lowerleft[1]+py		// upper right
	};
	memcpy(map.uvs, uvs, 8 * sizeof(float));
	return map;
}

geomap_ geomapMakeFromSubSpriteSheet(subspritesheet subsheet, unsigned int x, unsigned int y) {
	spritesheet sheet = subsheet.spritesheet;
}

void geomapDestroy(geomap_* map) {
	free(map->vertices);
	free(map->uvs);
	map->numfloats = 0;
}

#pragma mark -
#pragma mark Convenient Destruction

void destroy(geomap_* map) {
	geomapDestroy(map);
}
