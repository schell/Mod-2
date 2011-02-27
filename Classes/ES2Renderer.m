//
//  ES2Renderer.m
//  Mod-2
//
//  Created by Schell Scivally on 2/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ES2Renderer.h"
#import "PVRTexture.h"
#import "SZGLTextureLoader.h"
#import "SSGL.h"

GLuint uniforms[SSUniformCount];

enum {
	TEXTURE_ASTRONAUT,
	TEXTURE_BARBER_POLE,
	TEXTURE_GRASS,
	TEXTURE_FOREST_TILES,
	NUM_TEXTURES
};
PVRTexture* bgtex;
PVRTexture* texture;

@interface ES2Renderer (PrivateMethods)
- (BOOL)loadShaders;
- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file;
- (BOOL)linkProgram:(GLuint)prog;
- (BOOL)validateProgram:(GLuint)prog;
- (void)loadTextures;
@end

@implementation ES2Renderer

// Create an OpenGL ES 2.0 context
- (id)init
{
    self = [super init];
    if (self)
    {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

        if (!context || ![EAGLContext setCurrentContext:context] || ![self loadShaders])
        {
            [self release];
            return nil;
        }
		
        // Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
        glGenFramebuffers(1, &defaultFramebuffer);
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		[self loadTextures];
    }
    return self;
}

#pragma mark -
#pragma mark Textures

- (void)loadTextures {
	NSString* path = [[NSBundle mainBundle] pathForResource:@"tiles" ofType:@"pvr"];
//	CGSize imageSize = CGSizeMake(600, 600);
//	CGSize textureSize = CGSizeMake(600, 600);
//	textures[TEXTURE_FOREST_TILES] = SZGLLoadTexture(path, &imageSize, &textureSize);
	texture = [[PVRTexture pvrTextureWithContentsOfFile:path] retain];
	path = [[NSBundle mainBundle] pathForResource:@"bg" ofType:@"pvr"];
	bgtex = [[PVRTexture pvrTextureWithContentsOfFile:path] retain];
}

#pragma mark -
#pragma mark Sprites



#pragma mark -
#pragma mark Render

- (void)setMatrixUniformsWithProjection:(mat4*)projection andModelView:(mat4*)modelview {
	glUniform1f(uniforms[SSUniformSampler], 0);
	glUniformMatrix4fv(uniforms[SSUniformProjection], 1, GL_FALSE, projection[0]);
	glUniformMatrix4fv(uniforms[SSUniformModelView], 1, GL_FALSE, modelview[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

- (void)drawGeom:(geomap_)geometry {
	// Update attribute values
    glVertexAttribPointer(SSAttributeVertex, 2, GL_FLOAT, 0, 0, &geometry.vertices[0]);
    glEnableVertexAttribArray(SSAttributeVertex);
    glVertexAttribPointer(SSAttributeTexCoord, 2, GL_FLOAT, 1, 0, &geometry.uvs[0]);
    glEnableVertexAttribArray(SSAttributeTexCoord);
    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry.numfloats/2.0);
}

- (void)drawTiledSprite:(sprite)s withSize:(CGSize)size andProjectionMatrix:(mat4*)projection andModelViewMatrix:(mat4*)modelview {
	geomap_ geom = geomapMakeTiled(s, size.width, size.height);
	
	// texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, s.texture);
	
	[self setMatrixUniformsWithProjection:projection andModelView:modelview];
	[self drawGeom:geom];
	destroy(&geom);
}

- (void)drawSpriteFromSheet:(spritesheet)s withX:(unsigned int)x andY:(unsigned int)y andProjectionMatrix:(mat4*)projection andModelViewMatrix:(mat4*)modelview {
	geomap_ geom = geomapMakeFromSpriteSheet(s, x, y);
	
	// texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, s.sprite.texture);
	
	[self setMatrixUniformsWithProjection:projection andModelView:modelview];
	[self drawGeom:geom];
	destroy(&geom);
}

- (void)drawSprite:(sprite)s withProjectionMatrix:(mat4*)projection andModelViewMatrix:(mat4*)modelview {
	geomap_ geom = geomapMake(s);
	
	// texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, s.texture);
	
	[self setMatrixUniformsWithProjection:projection andModelView:modelview];
	[self drawGeom:geom];	
	destroy(&geom);
}

static BOOL logged = NO;
- (void)render
{
   // Replace the implementation of this method to do your own custom drawing
	float halfwidth = backingWidth/2.0;
	float halfheight = backingHeight/2.0;
	
	mat4 projection;
	mat4LoadOrtho(&projection,-halfwidth, halfwidth, halfheight, -halfheight, -1.0, 1.0);
	mat4 modelview;
	mat4LoadIdentity(&modelview);
	
	sprite bg = spriteMake([bgtex width], [bgtex height], bgtex.name);
	sprite tileset = spriteMake([texture width], [texture height], texture.name);
	spritesheet sheet = spritesheetMake(tileset, 43, 43, 3, 3);

    // This application only creates a single context which is already set current at this point.
    // This call is redundant, but needed if dealing with multiple contexts.
    [EAGLContext setCurrentContext:context];

    // This application only creates a single default framebuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple framebuffers.
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use shader program
    glUseProgram(program);

    // Validate program before drawing. This is a good check, but only really necessary in a debug build.
    // DEBUG macro must be defined in your debug configurations if that's not already the case.
#if defined(DEBUG)
    if (![self validateProgram:program])
    {
        NSLog(@"Failed to validate program: %d", program);
        return;
    }
#endif
	
	mat4Scale(&projection, 1.0, -1.0, 1.0);
	[self drawTiledSprite:bg withSize:CGSizeMake(backingWidth, backingHeight) andProjectionMatrix:&projection andModelViewMatrix:&modelview];
	//	mat4Translate(&modelview, 200, 0, 0);
	unsigned int map[16] = {1,1,1,1,
							4,0,0,4,
							6,6,6,6};
	for (int n = 0; n < 16; n++) {
		int x = map[n] % sheet.width;
		int y = map[n] / sheet.width;
		if (!logged) {
			NSLog(@"%s %i %i",__FUNCTION__,x,y);
		}
		mat4LoadIdentity(&modelview);
		mat4Translate(&modelview, (n%4)*sheet.framewidth, (n/4)*sheet.frameheight, 0);
		[self drawSpriteFromSheet:sheet withX:x andY:y andProjectionMatrix:&projection andModelViewMatrix:&modelview];
	}
	logged = YES;
	//[self drawSprite:tileset withProjectionMatrix:&projection andModelViewMatrix:&modelview];
	// This application only creates a single color renderbuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple renderbuffers.
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

#pragma mark -
#pragma mark Setup

- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
{
    GLint status;
    const GLchar *source;

    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source)
    {
        NSLog(@"Failed to load vertex shader");
        return FALSE;
    }

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(*shader);
        return FALSE;
    }

    return TRUE;
}

- (BOOL)linkProgram:(GLuint)prog
{
    GLint status;

    glLinkProgram(prog);

#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
#endif

    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0)
        return FALSE;

    return TRUE;
}

- (BOOL)validateProgram:(GLuint)prog
{
    GLint logLength, status;

    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }

    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0)
        return FALSE;

    return TRUE;
}

- (BOOL)loadShaders
{
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;

    // Create shader program
    program = glCreateProgram();

    // Create and compile vertex shader
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname])
    {
        NSLog(@"Failed to compile vertex shader");
        return FALSE;
    }

    // Create and compile fragment shader
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname])
    {
        NSLog(@"Failed to compile fragment shader");
        return FALSE;
    }

    // Attach vertex shader to program
    glAttachShader(program, vertShader);

    // Attach fragment shader to program
    glAttachShader(program, fragShader);

    // Bind attribute locations
    // this needs to be done prior to linking
    glBindAttribLocation(program, SSAttributeVertex, "position");
    glBindAttribLocation(program, SSAttributeTexCoord, "texcoord");

    // Link program
    if (![self linkProgram:program])
    {
        NSLog(@"Failed to link program: %d", program);

        if (vertShader)
        {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader)
        {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (program)
        {
            glDeleteProgram(program);
            program = 0;
        }
        
        return FALSE;
    }

    // Get uniform locations
    uniforms[SSUniformSampler] = glGetUniformLocation(program, "sampler");
    uniforms[SSUniformProjection] = glGetUniformLocation(program, "projection");
    uniforms[SSUniformModelView] = glGetUniformLocation(program, "modelview");

    // Release vertex and fragment shaders
    if (vertShader)
        glDeleteShader(vertShader);
    if (fragShader)
        glDeleteShader(fragShader);

    return TRUE;
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer
{
    // Allocate color buffer backing based on the current layer size
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }

    return YES;
}

- (void)dealloc
{
    // Tear down GL
    if (defaultFramebuffer)
    {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }

    if (colorRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }

    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }

    // Tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];

    [context release];
    context = nil;

    [super dealloc];
}

@end
