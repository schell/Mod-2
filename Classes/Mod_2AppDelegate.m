//
//  Mod_2AppDelegate.m
//  Mod-2
//
//  Created by Schell Scivally on 2/21/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "Mod_2AppDelegate.h"
#import "EAGLView.h"

@implementation Mod_2AppDelegate

@synthesize window;
@synthesize glView;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions   
{
    [glView startAnimation];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [glView stopAnimation];
}

- (void)dealloc
{
    [window release];
    [glView release];

    [super dealloc];
}

@end
