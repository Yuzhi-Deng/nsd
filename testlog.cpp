//g++ -Wall -Werror -g -o testlog testlog.cpp logsystem/log.cpp
#include "logsystem/debug.h" 

int main() {
  const char *log_file = "testlog.txt"; 
  init_log(log_file);

  for (int i = 0; i < 100; i++)
    Log("Record %d\n", i);

  return 0;
}
