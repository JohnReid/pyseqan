import inner_classes as IC

#IC.DoubleOuter.Inner = IC.IntOuter.Inner

int_inner = IC.IntOuter.Inner()
double_inner = IC.DoubleOuter.Inner()

print type(int_inner)
print type(double_inner)
