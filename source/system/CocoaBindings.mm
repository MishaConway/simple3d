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



static unsigned char* RawDataFromUIImage( UIImage* image, unsigned int* pOutWidth, unsigned int* pOutHeight )
{
    //if (image == nil)
    //    NSLog(@"Do real error checking here");
    
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
}

NSString* BundlepathFromFilepath( const char* path )
{
    NSString* cocoafied_path = [NSString stringWithUTF8String: path];
    NSString* filename = [[cocoafied_path componentsSeparatedByString:@"/"] lastObject];
    NSArray* split = [filename componentsSeparatedByString:@"."];
    NSString* filename_without_extension = [[split objectAtIndex: 0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    NSString* extension = [[split objectAtIndex: 1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    NSString* dir_name = [cocoafied_path stringByDeletingLastPathComponent];
    return [[NSBundle mainBundle] pathForResource:filename_without_extension ofType:extension inDirectory:dir_name];
}


void SetCocoaBindings()
{
    Directory::SetGetFilesInDirectoryBlock(^std::vector<std::string> (const char* path ){
        NSArray* array  = [[NSFileManager defaultManager] contentsOfDirectoryAtPath: [[[NSBundle mainBundle] resourcePath] stringByAppendingString:[NSString stringWithUTF8String:path]] error:nil];
        
        std::vector<std::string> files;
        for( unsigned int i = 0; i < [array count]; i++ )
            files.push_back( std::string( (char*) [[array objectAtIndex:i] UTF8String] ) );
        return files; 
    });
    
    File::SetFileExistsBlock(^bool(const char *path) {
        NSString* bundle_path = BundlepathFromFilepath( path );
        return bundle_path ? true : false;
    });
    
    File::SetReadAllTextBlock( ^std::string ( const char* path){        
        std::string file_contents;
        NSString* bundle_path = BundlepathFromFilepath(path);
        if( bundle_path)
            file_contents = std::string([[NSString stringWithContentsOfFile:bundle_path encoding:NSUTF8StringEncoding error:nil] UTF8String]);
        return file_contents;
    });
    
    
    SetGetRegexMatchesBlock( ^std::vector<std::string> (const char* _str, const char* _pattern){
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
        
        //char** pMatches = new char* [[array count]+1];
        //char** pStartMatches = pMatches;
        
        std::vector<std::string> out_matches;        
        for( unsigned int i = 0; i < [array count]; i++ )
            out_matches.push_back( std::string([[array objectAtIndex:i] UTF8String]) );
        return out_matches;            
    });
    
    OpenGLTexture::SetOnLoadTextureFileBlock(^unsigned char* (const char* cstr_path, unsigned int *pOutWidth, unsigned int *pOutHeight) {            
        NSArray* path_split = [[[NSString alloc] initWithUTF8String:cstr_path] componentsSeparatedByString:@"/"];
        NSString* filename = [path_split lastObject];
        NSString* directory = @"/";
        for( unsigned int i = 0; i < [path_split count] - 1; i++ )
        {
            if( 0 != [[path_split objectAtIndex:i] caseInsensitiveCompare:@""] )
            {
              //  NSLog( @"pathsplit here is %@\n", [path_split objectAtIndex:i] );
                directory = [directory stringByAppendingString:[path_split objectAtIndex:i]];
                directory = [directory stringByAppendingString:@"/"];
            }
        }
        
        
        
        NSArray* split = [filename componentsSeparatedByString:@"."];
        NSString* filename_without_extension = [[split objectAtIndex:0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        NSString* extension = [[split lastObject] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
        
      //  NSLog( @"directory is %@\n", directory );
      //  NSLog( @"extension is %@\n", extension );
      //  NSLog( @"filename without is %@\n", filename_without_extension );
        
        NSString* bundle_path = [[NSBundle mainBundle] pathForResource:filename_without_extension ofType:extension inDirectory:directory];
        
       // NSLog( @"bundle path is %@\n", bundle_path );
        
     /*   NSArray *dirContents  = [[NSFileManager defaultManager]
         directoryContentsAtPath:
         [[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/assets/video_thumbnails"]];
        
        NSLog( @"size of dir contents is %d\n", [dirContents count] );
        for( NSString* f in dirContents )
            NSLog( @"file is: %@\n", f ); */
        
        
        NSData *texData = [[NSData alloc] initWithContentsOfFile:bundle_path];
        UIImage *image = [[UIImage alloc] initWithData:texData];
        
        return RawDataFromUIImage( image, pOutWidth, pOutHeight );
    });
    
    OpenGLTexture::SetCreateTextureFromTextBlock(^unsigned char *(const char *_font, const unsigned int font_size, const unsigned int r, const unsigned int g, const unsigned int b, const unsigned int a, const char *_text, unsigned int *pOutWidth, unsigned int *pOutHeight) {
        
        CGSize imageSize;
        imageSize.width = 32;
        imageSize.height = 32;
        
        NSString* text = [[NSString alloc] initWithUTF8String:_text];
        
        
        // Create a bitmap graphics context of the given size
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGContextRef context = CGBitmapContextCreate(NULL, imageSize.width, imageSize.height, 8, imageSize.width*4, colorSpace, kCGImageAlphaPremultipliedLast);
        CGColorSpaceRelease(colorSpace);
        if (context== NULL) {
            return nil;
        }
        
        // Custom CGContext coordinate system is flipped with respect to UIView, so transform, then push
        CGContextTranslateCTM(context,0,imageSize.height);
        CGContextScaleCTM(context, 1.0, -1.0);
        UIGraphicsPushContext(context);
        
        // Inset the text rect then draw the text
        CGRect textRect = CGRectMake(4, 2, imageSize.width - 8, imageSize.height - 8);
        UIFont *font = [UIFont boldSystemFontOfSize:24];
        
        UIColor* text_color = [UIColor colorWithRed:((float)r / 255.0f) green:((float)g / 255.0f) blue:((float)b / 255.0f) alpha:((float)a/255.0f)];
        [text_color set];
       // [text drawInRect:textRect withFont:font
       //    lineBreakMode:UILineBreakModeTailTruncation alignment:UITextAlignmentCenter];
        
        // Create and return the UIImage object
        CGImageRef cgImage = CGBitmapContextCreateImage(context);
        UIImage *uiImage = [[UIImage alloc] initWithCGImage:cgImage];
        UIGraphicsPopContext();
        CGContextRelease(context);
        CGImageRelease(cgImage);
        
        return RawDataFromUIImage( uiImage, pOutWidth, pOutHeight );
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
        
        // Write image to PNG
        NSError* error;
        BOOL success = [UIImagePNGRepresentation(image) writeToFile:path options:NSDataWritingAtomic error:&error];
        if( !success )
            NSLog( @"failed to save image: %@\n", error );
            
        CGImageRelease( imageRef );
        CGDataProviderRelease( provider );
        CGColorSpaceRelease( colorSpaceRef );
    });
}

#endif



