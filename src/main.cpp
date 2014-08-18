#include "clang/AST/Type.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include<fstream>

#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Lexer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/AST/ParentMap.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
using namespace clang;
using namespace std;

// function for checking option arguments
void checkOptionArgument(int argc, char *argv[], set<string> &includeDirSet,
                         bool &isBoundCheck) {
  int opt = 0;
  const char *optString = ":I:hb";
  string includeDir;
  while((opt = getopt(argc, argv, optString)) != -1) {
    switch(opt) {
      case 'I':
        includeDir.assign(optarg);
        includeDir = "-I" + includeDir;
        //no need to check include options and no back up
        includeDirSet.insert(includeDir);
        break;
      case 'h':
        llvm::errs() << "This program should be used like this.\n";
        llvm::errs() << "option -I to set the include directory.\n";
        llvm::errs() << "option -b to invoke the boundcheck.\n";
        llvm::errs() << "option -h to get help.\n";
        exit(1);
        break;
      case 'b':
        isBoundCheck = true;
        break;
      case ':':
        llvm::errs() << "The option: " << (char)optopt << "need a value.\n";
        exit(1);
        break;
      case '?':
        llvm::errs() << "Unknown option: " << (char)optopt << "\n";
        exit(1);
        break;
      default:
        break;
    }
  }
}

// funtion for checking dir or file argument provided
int checkDirFile(string dirFilePath) {
  //check whether the arguments are dirs and exist
  int checkResult = 0;
  struct stat statBuf;

  if(stat(dirFilePath.c_str(), &statBuf) < 0) {
    checkResult = 0;
  } else if(S_ISDIR(statBuf.st_mode)) {
    checkResult = 1;
  } else if(S_ISREG(statBuf.st_mode)) {
    checkResult = 2;
  } else {
    checkResult = 0;
  }
  
  return checkResult;
}

void copyFile(string filePath, string desPath, set<string> &filePathSet,
                set<string> &allPathSet) {
  allPathSet.insert(desPath);
  if(desPath.rfind(".c") == (desPath.size() - 2)||desPath.rfind(".h") == (desPath.size() - 2)) {
    filePathSet.insert(desPath);
//    llvm::errs()<<desPath.rfind(".c")<<" "<<desPath.size()<<"  "<<desPath<<"  filePathSet\n";
  }
  int sfd = 0, dfd = 0;
  int len = 0;
  char buff[512] = {0};
  sfd = open(filePath.c_str(), O_RDONLY);
  if(sfd < 0) {
    llvm::errs() << "open file " << filePath << " failed.\n";
    exit(1);
  }
  dfd = open(desPath.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  if(dfd < 0) {
    llvm::errs() << "open file " << desPath << " failed.\n";
    exit(1);
  }
  do {
    len = read(sfd, buff, sizeof(buff));
    if(len > 0) {
      len = write(dfd, buff, len);
    }
  } while(len > 0);
  close(sfd);
  close(dfd);
}

// copy directories to /tmp
void copyDir(string dirPath, string desPath, set<string> &includeDirSet,
               set<string> &filePathSet, set<string> &allPathSet) {
  size_t posFoundLow = desPath.rfind("/include");
  size_t posFoundUp = desPath.rfind("/INCLUDE");
  if(posFoundLow == desPath.size() - 8 || posFoundUp == desPath.size() - 8) {
    string opIncludeDir = "-I" + desPath;
    includeDirSet.insert(opIncludeDir);
  }
  struct dirent *pEnt = NULL;
  DIR *pSrcDir = opendir(dirPath.c_str());
  if(pSrcDir == NULL) {
    llvm::errs() << "open directory " << dirPath << " failed.\n";
    exit(1);
  }
  if(!opendir(desPath.c_str())) { 
    if(mkdir(desPath.c_str(), S_IRWXU) != 0) {
      llvm::errs() << "error  mkdir " << desPath << " .\n";
      exit(1);
    }
  }
  string dirName, nDirPath, nDesPath;
  while((pEnt = readdir(pSrcDir)) != NULL) {
    switch(pEnt->d_type) {
      case DT_REG:
        nDirPath = dirPath + "/" + pEnt->d_name;
        nDesPath = desPath + "/" + pEnt->d_name;
        copyFile(nDirPath, nDesPath, filePathSet, allPathSet);
        break;
      case DT_DIR:
        dirName = pEnt->d_name;
        if(dirName.compare(".") != 0 && dirName.compare("..") != 0) {
          nDirPath = dirPath + "/" + pEnt->d_name;
          nDesPath = desPath + "/" + pEnt->d_name;
          copyDir(nDirPath, nDesPath, includeDirSet, filePathSet, allPathSet);
        }
        break;
      default:
        break;
    }
  }
  closedir(pSrcDir);
}

void rmDir(string dirPath) { 
  struct dirent *pEnt = NULL;
  DIR *pSrcDir = opendir(dirPath.c_str());
  if(pSrcDir == NULL) {
    return;
  }
  string nDirPath;
  while((pEnt = readdir(pSrcDir)) != NULL) {
    switch(pEnt->d_type) {
      case DT_DIR:
        nDirPath = pEnt->d_name;
        if(nDirPath.compare(".") != 0 && nDirPath.compare("..") != 0) {
          nDirPath = dirPath + "/" + nDirPath;
          rmDir(nDirPath);
        }
        break;
      default:
        nDirPath = dirPath + "/" + pEnt->d_name;
        if(remove(nDirPath.c_str()) != 0) {
          llvm::errs() << "delete error.\n";
        }
        break;
    }
  }
  closedir(pSrcDir);
  if(remove(dirPath.c_str()) != 0) {
    llvm::errs() << "delete error.\n";
  }
}

// function for checking file arguments
void checkFileDirArgument(int argc, char *argv[],
                          set<string> &includeDirSet, set<string> &filePathSet,
                          set<string> &allPathSet) {
  for(;optind < argc; ++optind) {
    string dirFilePath = argv[optind];
    llvm::errs()<<dirFilePath<<"  KKKKKKKK\n";
    if(checkDirFile(dirFilePath) == 0) {
      llvm::errs() << "Please check your arguments, file or diectory not exist.\n";
      exit(1);
    }
    size_t ext = dirFilePath.rfind("/");
    if(ext == dirFilePath.size() - 1 && ext != 0) {
      dirFilePath.erase(ext, 1);
      ext = dirFilePath.rfind("/");
    }
    llvm::errs()<<dirFilePath<<"  2KKKKKKKK\n";
    string dirPart, filePart;// 目录部分，文件部分
    if(ext == string::npos) {
      dirPart = dirFilePath;
    } else {
      dirPart = dirFilePath.substr(0, ext);
      filePart = dirFilePath.substr(ext, dirFilePath.size() - ext);
    }

    //first get the actual path of dirFilePath
    char curDir[512] = {0};
    getcwd(curDir, sizeof(curDir));
    string curDirPath = curDir;
    llvm::errs()<<curDirPath<<"  curDirPath\n";
    chdir(dirPart.c_str());
    getcwd(curDir, sizeof(curDir));
    llvm::errs()<<curDir<<"  curDir\n";
    chdir(curDirPath.c_str());
    dirPart = curDir;
    dirFilePath = dirPart + filePart;
    llvm::errs()<<dirFilePath<<"  dirFilePath\n";
    //end get the actual path of dirFilePath

/*  
    string desPath = dirFilePath;
    ext = desPath.rfind("/");
    if(ext != std::string::npos) {
      desPath.erase(0, ext + 1);
    }
    llvm::errs()<<desPath<<"  desPath\n";
    desPath = "/tmp/" +desPath;
*/
    string desPath=dirFilePath+"_out";
    llvm::errs()<<desPath<<"  LLLL\n";
//    desPath.append("_out");
    switch(checkDirFile(dirFilePath)) {
      // directory
      case 1:
        rmDir(desPath);
        copyDir(dirFilePath, desPath, includeDirSet, filePathSet, allPathSet);
        break;
      // regular file
      case 2:
        remove(desPath.c_str());
        copyFile(dirFilePath, desPath, filePathSet, allPathSet);
        break;
      default:
        break;
    }
  }
}


void backupMacroedFiles(set<string> &allPathSet) {
  for(set<string>::iterator itPath = allPathSet.begin();
        itPath != allPathSet.end(); ++itPath) {
    string filePath = *itPath;
    string desPath = filePath + ".tmp";
    set<string> temp1,temp2;
    copyFile(filePath, desPath, temp1, temp2);
  }
}

int main(int argc, char **argv)
{
  struct stat sb;
  if (argc < 2)
  {
     llvm::errs() << "Usage: CIrewriter <options> <filename>\n";
     return 1;
  }

  bool isBoundCheck = false;
  

  // initalize the vector for files and includeDir
  std::set<std::string> includeDirSet;
  std::set<std::string> filePathSet;
  std::set<std::string> allPathSet;
  
  // check the option arguments
  checkOptionArgument(argc, argv, includeDirSet,
                      isBoundCheck);

  // check the file arguments
  checkFileDirArgument(argc, argv, includeDirSet, filePathSet, allPathSet);

  if(isBoundCheck)
  {
     llvm::errs()<<"BoundCheck is begining.\n";
//     boundcheck(includeDirSet,filePathSet);
  }
}
