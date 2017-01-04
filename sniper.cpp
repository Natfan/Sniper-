#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include "rapidjson/document.h"
#include <cstring>
#include <chrono>
#include "gumbo.h"
#include <thread>

#define MAX_THREADS 4

// CREDITS TO DOGGIEKUK <-- best cod3r xddddddd & OMAR H FOR HELPING <3

// g++ sniper.cpp -o xd -l curl -std=c++11 `pkg-config --cflags --libs gumbo`
//
using namespace std;
using namespace rapidjson;
//
bool found = false;
//
string auth;
//
string currentUserName;
//
string uuidURL;
//
string payLoad;
//
std::string UUID;
//
string username = "";
//
string email = "";
//
string password = "";
//
string desired = "";
//
CURL* curlChange;
//
CURLcode resChange;
//
string endBuff;
//
static void scanForAuth(GumboNode* node) {
  if (found) {
    return;
  }
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* href;
  GumboAttribute* hidden;
  if (node->v.element.tag == GUMBO_TAG_INPUT &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "value"))) {
    std::cout << "[SNIPER++]: Authentication code: " << href->value
              << std::endl;
    found = true;
    auth = href->value;
    std::stringstream post;
    post << "newName=" << desired << "&password=" << password
         << "&authenticityToken=" << auth;
    payLoad = post.str();
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    scanForAuth(static_cast<GumboNode*>(children->data[i]));
  }
}
//
static size_t WriteCallback(void* contents, size_t size, size_t nmemb,
                            void* userp) {
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}
//
void prepareCurl() {
      if (curlChange) {
    curl_easy_setopt(curlChange, CURLOPT_URL, uuidURL.c_str());
    curl_easy_setopt(curlChange, CURLOPT_POSTFIELDS, payLoad.c_str());
    curl_easy_setopt(curlChange, CURLOPT_COOKIEJAR, "cookie.txt");
    curl_easy_setopt(curlChange, CURLOPT_COOKIEFILE, "cookie.txt");
    curl_easy_setopt(curlChange, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curlChange, CURLOPT_WRITEDATA, &endBuff);
    curl_easy_setopt(curlChange, CURLOPT_FOLLOWLOCATION, 1);
      }
}
//
void changeName() {
    resChange = curl_easy_perform(curlChange);
    curl_easy_cleanup(curlChange);
    cout << "[SNIPER++] " << endBuff << endl;
}
//
void getAuthentication() {
  CURL* curl;
  curl = curl_easy_init();
  std::string readBuffer;
  std::stringstream ss;
  ss << "https://account.mojang.com/me/renameProfile/" << UUID;
  std::string urlFix = ss.str();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, urlFix.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookie.txt");
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    GumboOutput* output = gumbo_parse(readBuffer.c_str());
    scanForAuth(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
        prepareCurl();
  }
}
//
size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp) {
  return size * nmemb;
}
//
void login() {
  CURL* curl;
  curl = curl_easy_init();
  std::stringstream ss;
  ss << "username=" << email << "&password=" << password;
  std::string postdata = ss.str();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://account.mojang.com/login");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookie.txt");
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie.txt");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    cout << "[SNIPER++]: Logged in (!)" << endl;
    getAuthentication();
  }
}
//
void getUUID() {
  CURL* curl;
  curl = curl_easy_init();
  std::string readBuffer;
  std::stringstream ss;
  ss << "https://api.mojang.com/users/profiles/minecraft/" << username;
  std::string urlFix = ss.str();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, urlFix.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    Document document;
    document.Parse(readBuffer.c_str());
    UUID = document["id"].GetString();
    cout << "[SNIPER++]: Found the UUID: " << UUID << std::endl;
    std::stringstream ss;
    ss << "https://account.mojang.com/me/renameProfile/" << UUID;
    uuidURL = ss.str();
    login();
  }
}
//
bool fired = false;
bool gotsnipe = false;
long snipetime;

void startLoop() {
  for (;;) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long mslong =
        (long long)tp.tv_sec * 1000L +
        tp.tv_usec / 1000; 
    
    if (fired) {
      //  return voidPtr;
      return;
    }
    if (mslong + 2 > snipetime) {
      changeName();
      fired = true;
      cout << "[SNIPER++]: Fired the bullet (!)" << endl;
    }
  }
}
//
int main() {
    
  pthread_t threads[MAX_THREADS];
    int i;
  curlChange = curl_easy_init();
  remove("cookie.txt");

  struct tm t = {0};
  t.tm_year = 116;
  t.tm_mon = 11;
  t.tm_mday = 5;
  t.tm_hour = 20;
  t.tm_min = 4;
  t.tm_sec = 8;
  time_t timeSinceEpoch = mktime(&t);
  snipetime = (long)timeSinceEpoch * 1000;
  cout << "[SNIPER++]: Retrieved EPOCH: " << timeSinceEpoch * 1000 << endl;
  cout << "[SNIPER++]: Prepared to snipe (!)" << endl;
  getUUID();
  std::thread t1(startLoop);  
  t1.join();
  
  
   // for(i = 0; i < MAX_THREADS; i++) {
     //   cout << "[SNIPER++]: Created thread: " << i << " - MAX THREADS: " << MAX_THREADS << endl;
     //   pthread_create(&threads[i], NULL, startLoop, NULL);
    //}
    
}
