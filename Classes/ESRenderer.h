//
//  ESRenderer.h
//  Mod-2
//
//  Created by Schell Scivally on 2/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

@protocol ESRenderer <NSObject>

- (void)render;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;

@end
