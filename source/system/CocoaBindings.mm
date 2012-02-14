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
        
        GLuint original_width = CGImageGetWidth(image.CGImage);
        GLuint original_height = CGImageGetHeight(image.CGImage);
        
        unsigned int power_of_two = 1;
        while( power_of_two < original_width )
            power_of_two *= 2;
        GLuint width = power_of_two;
        
        power_of_two = 1;
        while( power_of_two < original_height  )
            power_of_two *= 2;
        GLuint height = power_of_two; 
        
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
    
    OpenGLTexture::SetOnSaveTextureFileBlock( ^(const char* cpath, unsigned char* data, const unsigned int width, const unsigned int height)
    {
        // make data provider with data.
        CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, data, width*height*4, NULL);
        
        // prep the ingredients
        int bitsPerComponent = 8;
        int bitsPerPixel = 32;
        int bytesPerRow = 4 * width;
        CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
        CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
        
        // make the cgimage
        CGImageRef imageRef = CGImageCreate(width, height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
        
        // then make the uiimage from that
        UIImage* image = [UIImage imageWithCGImage:imageRef];
       // return myImage;
        
        //NSString  *pngPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/Test.png"];
        //NSString  *jpgPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/Test.jpg"];
        
        NSString* path = [NSString stringWithUTF8String: cpath];
        
        // Write a UIImage to JPEG with minimum compression (best quality)
        // The value 'image' must be a UIImage object
        // The value '1.0' represents image compression quality as value from 0.0 to 1.0
     //   [UIImageJPEGRepresentation(image, 1.0) writeToFile:path atomically:YES];
        
        // Write image to PNG
        [UIImagePNGRepresentation(image) writeToFile:path atomically:YES];
    });
}

#endif



