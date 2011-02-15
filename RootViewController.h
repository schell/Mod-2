//
//  RootViewController.h
//  Mod-1
//
//  Created by Schell Scivally on 1/25/11.
//  Copyright 2011 Electrunique. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RootAudioController.h"
#import "UIModularUnitViewController.h"
#import "UIModularUnitMenuController.h"

@interface RootViewController : UIViewController {
	RootAudioController* _rootAudioController;
	UIModularUnitViewController* _headUnitViewController;
	UIModularUnitMenuController* _menuController;
	UITapGestureRecognizer* _twoFingerSingleTap;
}
+ (CGSize)portraitSize;
+ (CGSize)landscapeSize;
- (void)didSingleTapWithTwoFingers:(UITapGestureRecognizer*)tap;
@end
