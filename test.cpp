#include "tinyxml2.h"
using namespace  std;
using namespace tinyxml2;

class  vector
{
public:
    vector(){
      length = 0;
    }
    int Length(){
      return length;
    }
    bool push(XMLElement*  e){
      if (e == nullptr)
        return false;
      data[length] = e;
      length++;
      return true;
    }
    XMLElement* pop(){
      if(!length)
        return nullptr;
      length --;
      return data[length];
    }
private:
    XMLElement* data[10];
    int length;
};

void parseAttribute(XMLElement* e){
  const XMLAttribute* attr = e->FirstAttribute();
  if( attr != nullptr){
    printf( "%s=\"%s\" ", attr->Name(),attr->Value());
    while( ( attr = attr->Next()) != nullptr ){
        printf( "%s=\"%s\" ", attr->Name(),attr->Value());
    }
  }
}

/**
 * 1.获取根结点root之后,解析根结点的属性
 * 2.获取第一个子节点parent，并解析子节点属性。
 * 3.判断子节点child存不存在，如果存在把父节点parent压栈，对child进行解析
 * 4.如果child没有子节点，遍历兄弟节点brother进行解析
 * 5.兄弟节点也解析完，出栈parent，获取parent的兄弟节点，在判断子节点。。。循环
 * 6.栈内没有数据，最上层的兄弟节点解析完，退出。
 * */

int main(){
  
  vector* v =  new vector();
  XMLDocument doc;
  doc.LoadFile("pp.xml");
  XMLElement* element = doc.RootElement();
  const char * text = nullptr;
  while (element != nullptr || v->Length())
  {
    //节点为空，出栈父节点
    if(element == nullptr){
      XMLElement* node = v->pop();
      printf("</%s>\n",node->Name());
      element = node->NextSiblingElement();
    } else {
      //节点不为空，可以进行解析
      printf("<%s ", element->Name());
      parseAttribute(element);
      if(element == doc.RootElement()){
        printf(">\n");
        element = element->FirstChildElement();
        continue;
      }
      //子节点不为空，将该节点进行压栈
      if (element->FirstChildElement() != nullptr){
        printf(">\n");
        v->push(element);
        element = element->FirstChildElement();
        continue;
      } else if ((text = element->GetText()) != nullptr){//判断有没有文本
         printf(">");
        printf("%s",text);
        printf("</%s>\n",element->Name());
      } else 
          printf("/>\n");
      element = element->NextSiblingElement();//默认获取兄弟节点，有子节点，子节点先处理，没有子节点处理兄弟节点，没有兄弟节点，处理父的兄弟节点。。。类推
    } 
  }

  element = doc.RootElement();
  printf("</%s>\n",element->Name());
  return 0;
}

