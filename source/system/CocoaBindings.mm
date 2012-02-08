//
//  CocoaBindings.m
//  Simple3D_iOS
//
//  Created by Misha Conway on 2/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#if defined(__APPLE__) || defined(__APPLE_CC__) 

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import "CocoaBindings.h"
#include "FileIO.h"
#include "../string/string_utils.h"
#include "../graphics/opengl/OpenGLTexture.h"

void SetCocoaBindings()
{
    File::SetReadAllTextBlock( ^const char*( const char* path){
        NSString* filename = [[[NSString stringWithUTF8String: path] componentsSeparatedByString:@"/"] lastObject];
        
        NSArray* split = [filename componentsSeparatedByString:@"."];
        NSString* filename_without_extension = [[split objectAtIndex: 0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        NSString* extension = [[split objectAtIndex: 1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        
        NSString* file_path = [[NSBundle mainBundle] pathForResource:filename_without_extension ofType:extension ];   
        
        //  NSLog( @"here filepath is %@", file_path );
        
        
        return [[NSString stringWithContentsOfFile:file_path encoding:NSUTF8StringEncoding error:nil] UTF8String]; 
    });
    
    SetGetRegexMatchesBlock( ^char** (const char* _str, const char* _pattern){
        NSString* str = [NSString stringWithUTF8String:_str];
        NSString* pattern = [NSString stringWithUTF8String:_pattern];
        
        NSError* error = nil;
        NSRegularExpression* regex = [NSRegularExpression 
                                      regularExpressionWithPattern:pattern options:0 error:&error];
        
        NSArray* matches = [regex matchesInString:str 
                                          options:0 range:NSMakeRange(0, [str length])];
        
        NSMutableArray* array = [[NSMutableArray alloc] init];  
        for ( NSTextCheckingResult* match in matches )
        {
            NSString* match_text = [str substringWithRange:[match range]];
            [array addObject:match_text]; 
        }
        
        char** pMatches = new char* [[array count]+1];
        char** pStartMatches = pMatches;
        for( unsigned int i = 0; i < [array count]; i++ )
            *pMatches++ = (char*) [[array objectAtIndex:i] UTF8String];
        *pMatches = 0;
        
        return pStartMatches;            
    });
    
    OpenGLTexture::SetOnLoadTextureFileBlock(^unsigned char* (const char* cstr_path, unsigned int *pOutWidth, unsigned int *pOutHeight) {            
        NSString* path = [[[[NSString alloc] initWithUTF8String:cstr_path] componentsSeparatedByString:@"/"] lastObject];
        NSArray* split = [path componentsSeparatedByString:@"."];
        NSString* filename_without_extension = [[split objectAtIndex: 0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        NSString* extension = [[split objectAtIndex: 1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        
        NSString* bundle_path = [[NSBundle mainBundle] pathForResource:filename_without_extension ofType:extension];
        NSData *texData = [[NSData alloc] initWithContentsOfFile:bundle_path];
        UIImage *image = [[UIImage alloc] initWithData:texData];
        if (image == nil)
            NSLog(@"Do real error checking here");
        
        GLuint width = CGImageGetWidth(image.CGImage);
        GLuint height = CGImageGetHeight(image.CGImage);
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        unsigned char* imageData = new unsigned char[height * width * 4];
        CGContextRef context = CGBitmapContextCreate( imageData, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
        CGColorSpaceRelease( colorSpace );
        CGContextClearRect( context, CGRectMake( 0, 0, width, height ) );
        CGContextTranslateCTM( context, 0, height - height );
        CGContextDrawImage( context, CGRectMake( 0, 0, width, height ), image.CGImage );
        CGContextRelease( context );
        
        *pOutWidth = width;
        *pOutHeight = height;
        
        return imageData;
    });
}

#endif



