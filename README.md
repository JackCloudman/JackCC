# JackCC (Jack Complex Calculator)
Jack's Complex Calculator
This is a simple complex calculator using yacc and lex
## Information
### Strings
Show messages in your programs, now  you can put strings as:
```python
text =  "My awesome text"
```
Some operations are:
```python
text = "x :" + x # Convert complexnum to text
message = "hello "+ "world" # concatenate strings
```
### Arrays
Store multiples nums as!
```
arr =  [1,2,3,4+3i]
```
**Operations**
```
[1,2,3]+[4,5,6] #Merge arrays with operator '+'
arr[0] #Access to array data (index start in 0)
arr[-1] # Yes! you can use negative numbers to get data of right to left
arr[0] = 3 # Change array values 
```
**Duplicate arrays**
```
x = y = [1,2,3] #Copy same reference
x = [1,2,3] #To make different arrays merge second array with empty array
y = x+[ ] # x and y are different arrays with same values
```
### Loops
Exist two type of loops  
**for**
```
for(i=0;i<4;i=i+1){
  print i
}
```
**while**
```
while(i!=0){
  print i
  i=i-1;
}
```
### Functions
Define function:
```
def myfunc(){
  return $1+$2
}
```
Call function:
```
result = myfunc(5,4)
```
## Procedures
Define procedures:
```
proc myproc(){
  print "hello world!"
  x = x+1
}
```
Call procedures:
```
myproc();
```
### Input complex values
```python
var = input("Message")
```
### Constants
```
PI = 3.14159265358979323846
E = 2.71828182845904523536
GAMMA = 0.57721566490153286060
DEG" = 57.29577951308232087680
PHI = 1.6180339887498948*820
i = 0+1i
```
### Math functions
```
sen(complexnum)
cos(complexnum)
log(complexnum)
abs(complexnum)
tan(complexnum)
real(complexnum)
img(complexnum)
```
More functions in examples/math.jcc
