import Teracada as tc

def testModuleImport ():
  tcArray = tc.TeracadaArray()
  print(dir(tcArray))
  tcArray.print()
  tcArray.array([1, 2, 3], dtype=1)

  # print(tcArray.array([1, 2, 3]))
  print("--->", tcArray.get())
  print(tcArray)
  # print("====")

if __name__ == "__main__":
  testModuleImport()
