from functools import singledispatch

@singledispatch
def fun(str1):
  print("string ", str1)


@fun.register(int)
def _(val):
  val = val = 1
  print("int ", val)



if __name__ == '__main__':
  fun("nipun")
  fun(23)

