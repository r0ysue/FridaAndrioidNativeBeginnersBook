# import idautils
#
# for mangled in idautils.Functions():
#     demangled = idc.demangle_name(
#         str(idaapi.get_func_name(mangled)),
#         idc.get_inf_attr(idc.INF_SHORT_DN)
#     )
#
#     if demangled is not None:
#         print("{} => {}".format(mangled, demangled))







####
# 一点点小细节
# 1.命名粉碎会影响字符串中关键线索的查找，需要还原
#