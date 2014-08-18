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

//行列号信息
struct Array_location_info
{
   int line;
   int column;
};

//实际插桩函数个数
int insert_total;
//可插桩点个数
int insert_total2;
// 是否在循环体中
bool m_inForLine=false;

//指针定义时前一步的位置  
  SourceLocation previous=SourceLocation(); 

typedef std::vector<std::pair<ValueDecl *,ValueDecl *> >Pointer_vector;

// RecursiveASTVisitor is is the big-kahuna visitor that traverses
// everything in the AST.
class MyRecursiveASTVisitor: public ASTConsumer,public RecursiveASTVisitor<MyRecursiveASTVisitor>
{
public:
  MyRecursiveASTVisitor(Rewriter &R) : Rewrite(R) {p1=0;issame="";isConstantArray=false;insert_total=0;insert_total2=0;str_count=0;decl_count=0; }
  bool VisitVarDecl(VarDecl *D);
  void getlineandcolumn(SourceLocation Loc);
  std::string lookforlength(std::string ss,int len,int p11);
  bool declaresSameEntity(const ValueDecl *D1, const ValueDecl *D2);
  
  int lookfordimension(std::string ss);
  int lookforlastdimension(std::string ss,int dimen,int sum_dimen);
  
  bool isSameSourceLocation(SourceLocation SL1,SourceLocation SL2);
  bool VisitBinAssign(BinaryOperator *Bo);
  bool VisitUnaryDeref(UnaryOperator *Uo);
  Expr* lookinBinaryOperator(BinaryOperator *Bo);
  void insert_func(ValueDecl *VD1,SourceLocation SL1,SourceLocation SL2);
  bool VisitFunctionDecl(FunctionDecl *f);
  
  bool VisitCallExpr(CallExpr *CE);

  std::string calculate_storage(VarDecl *VD);
  bool TraverseForStmt(ForStmt *s);
  
  bool VisitDeclStmt(DeclStmt *E);
  bool VisitImplicitCastExpr(ImplicitCastExpr *E);
  bool VisitBinComma(BinaryOperator *stmt);
  bool VisitArraySubscriptExpr(ArraySubscriptExpr *AS);
  
  Rewriter &Rewrite;
//  CompilerInstance compiler;

private:
  Array_location_info Loc_info;
//得到固定数组的长度信息要用到的参数
  int p1;   
  SourceLocation SC2;
  SourceLocation SC3;
  SourceLocation SC1;
//首先遍历得到变量的缓冲区
  
  char fc4[256];
  char fc2[256];
 //插桩缓存区
  char fcd1[256];  
//用来存放全局变量的初始化
  char global[4096]; 

//字符串初始化时的父节点位置
  SourceLocation Parent_Loc;
  VarDecl *Dc;
//字符串计数变量
  int str_count;
//数组维数计数器
  int decl_count;
//变量定义的范围  
  SourceRange SR;
  ASTContext *Context; 
//issame用来判断是不是处理到下一个数组了
  std::string issame;
  std::string fname;
//开始访问的是第几维
  int dimension;
  
//判断传递过去的是不是固定数组 
  bool isConstantArray;   
  ValueDecl *look_pointer;
//把函数声明传递过来,判断以后的插桩的是不是参数
  FunctionDecl *global_FD;  
//形成指针序列
  std::vector<std::pair<ValueDecl *,ValueDecl *> >P_vector; 
 

};
