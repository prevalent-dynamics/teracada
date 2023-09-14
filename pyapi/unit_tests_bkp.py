import Teracada as tc

def testModuleImport ():
  tcArray = tc.TeracadaArray(dtype = 1, num = 10)
  print(dir(tcArray))
  print(tcArray.dtype())

  # print(tcArray.array([1, 2, 3]))

  # print(tcArray.print())
  # print("====")
  # print(tcArray)

if __name__ == "__main__":
  testModuleImport()
