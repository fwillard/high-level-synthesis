ECE574A Assignment 2:

Group Members:
  Name                NetID
  Michael Krotchie    krotchie   
  Finn Willard        fwillard
  Paul Niman          nimanp
  Sterling Davis      sdsilver

Program Description:
Dpgen takes in two arguments, the input circuit file and the name of the
output file. Dpgen then converts the input file to synthesizable verilog
code. Then the program determines the critical path of the circuit which 
is defined as the largest delay between either an input to a REG 
component, or a REG-REG component.

Contribution:
  Michael Krotchie    Verilog Output
  Finn Willard        Critical Path Cal, CMake Setup
  Paul Niman          planning and logic
  Sterling Davis      Parser and Cmake
