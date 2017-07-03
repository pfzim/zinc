@echo off

c:\pstools\cygwin\bin\flex.exe -L -ojson_lexer.cpp json_lexer.l
c:\pstools\cygwin\bin\bison.exe -r all -o json_parser.cpp json_parser.y

c:\pstools\cygwin\bin\flex.exe -L -ojson_strong_lexer.cpp json_strong_lexer.l
c:\pstools\cygwin\bin\bison.exe -r all -o json_strong_parser.cpp json_strong_parser.y

