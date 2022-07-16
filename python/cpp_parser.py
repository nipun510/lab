#! /usr/bin/env python

import sys
import argparse

class function:
  def __init__(self):
    self.name
    self.returnType 
    self.body
    self.args

  def parse_function_body(currPos):


  def parse_args():
    pass
  
  def parse(self):
    pass

class token:
  def __init__(self, name, ttype):
    self.name = name
    self.type = ttype


class lexicalParser:
  def __init__(self):
    self.tokens = []

  def parse():
    with  open(self.sourceFile) as f:
      for line in f:




class sourceFileParser:
  def __init__(self, file):
    self.sourceFile = file

  def parse(self):
    with  open(self.sourceFile) as f:
      for line in f:
        print(line)


if __name__ == '__main__':
  parser = argparse.ArgumentParser("parser")
  parser.add_argument('--file', required = True)
  args = parser.parse_args()
  obj = sourceFileParser(args.file)
  obj.parse()
