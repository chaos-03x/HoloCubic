import os


def print_directory_tree(path, prefix=''):
    # 获取当前路径下的所有文件和文件夹
    items = os.listdir(path)

    # 遍历所有项
    for item in items:
        # 跳过以 '.' 开头的文件或文件夹
        if item.startswith('.'):
            continue

        # 组合完整路径
        item_path = os.path.join(path, item)

        # 判断是文件还是文件夹
        if os.path.isfile(item_path):
            # 是文件，直接打印
            print(f"{prefix}├── {item}")
        elif os.path.isdir(item_path):
            # 是文件夹，打印后递归调用打印其子目录
            print(f"{prefix}└── {item}/")
            print_directory_tree(item_path, prefix + '    ')


# 输入要打印的文件夹路径
folder_path = "./"

# 调用函数打印文件目录树
print_directory_tree(folder_path)
