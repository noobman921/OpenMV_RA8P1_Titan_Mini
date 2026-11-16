# 冻结 ml 包 —— 由 src/MicroPython/tools/makemanifest.py 处理
#
# 用法见项目根目录的 build_frozen.sh。
#
# 关于 path 参数：makemanifest 在执行本文件时会把 CWD 切到本文件所在目录
# （即 frozen_ml/），且 freeze_as_mpy 的 path 是相对 CWD 解析的。
# 所以这里用 "."；模块名 = 相对 "." 的路径 = "ml/__init__.py"、"ml/model.py"……
# 与原来的 frozen_content.c 完全一致。
#
# 为什么不写 "$(FROZEN_DIR)"：manifestfile.py 的 _freeze_internal 判断
# "script 是不是目录" 时用的是**未做变量替换**的原始字符串，
# os.path.isdir("$(FROZEN_DIR)/ml") 必然为 False → 会把目录当文件打开而报错。
#
# 用 freeze_as_mpy 而不是 freeze：freeze() 走 KIND_FREEZE_AUTO，
# 而本工程的 makemanifest.py 只处理 AS_STR / AS_MPY / MPY 三种 kind，
# AUTO 会命中 assert；freeze_as_mpy 的 exts 只含 ".py"，会自动调 mpy-cross 编译，
# 并忽略 frozen_ml/ml/ 下既有的 .mpy（避免同名重复冻结）。
freeze_as_mpy(".", "ml")
