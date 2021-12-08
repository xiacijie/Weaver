#!/bin/bash
shopt -s expand_aliases

rm -rf build
mkdir build
export CLASSPATH=".:../bin/antlr4/antlr-4.9.2-complete.jar:$CLASSPATH"
alias antlr4='java -jar ../bin/antlr4/antlr-4.9.2-complete.jar'
alias grun='java org.antlr.v4.gui.TestRig'

antlr4 WeaverLexer.g4 WeaverParser.g4 -o ./build
cd build 
javac *.java 
grun Weaver program -gui ../../../example/test.wvr
