#ifndef CLIENT_UTIL_H
#define CLIENT_UTIL_H
#include <vector>
#include <string>
class Connection;
struct Message;

// this header file is useful for any declarations for functions
// or classes that are used by both clients (the sender and receiver)

std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

// you can add additional declarations here...
std::vector<std::string> tokenize(std::string s, std::string del);

#endif // CLIENT_UTIL_H
