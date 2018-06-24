//
//  main.cpp
//  CloudGaming
//
//  Created by Yong Yao on 12/1/14.
//  Copyright (c) 2014 yong. All rights reserved.
//

#include <iostream>


#include "apis.h"
int main0(int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

void TestMap ()
{
    std::map<int, int*> mymap;
    for (int i = 0; i < 5; i++)
        mymap[i] = &i;
    for (int i = 20; i >= 15; i--)
        mymap[i] = &i;
    std::map<int, int*>::reverse_iterator it;
    for (it = mymap.rbegin(); it != mymap.rend(); ++it)
        printf("%d ", *(it->second));

    if (mymap[100] == NULL)
    printf("mymap[100] not exit\n");
    
}

int main(int argc, char *argv[])
{

  signal( SIGPIPE, SIG_IGN );
  signal(SIGUSR1,SIG_IGN);
  signal(SIGVTALRM,SIG_IGN);

  start_init_texture_path("/Users/yong/Documents/3D_Vision/CloudGaming/v4/");
  //start_init_texture_path("./");

  start_with_windows_with_jpg(argc, argv, 600, 600);
  //start_by_glx_with_mp4("111", 400, 400);
  //start_by_glx_with_jpg("111", 600, 600);
  //start_by_glx_with_jpg_with_noblock("/Users/yong/Documents/3D_Vision/CloudGaming/v4/", 300, 300);
   return 1;
}

