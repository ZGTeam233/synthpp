# MIDI Scribe Tool

将 MIDI 文件解析为 JSON，提取所有音符的音高、开始时间、时长。

---

## 安装依赖

```bash
pip install pretty_midi mido -i https://pypi.tuna.tsinghua.edu.cn/simple
```

---

## 用法

```bash
python scribe_main.py [input.mid] [output.json] [options]
```

### 参数

| 参数 | 说明 |
|------|------|
| `input.mid` | 输入的 MIDI 文件 |
| `output.json` | 输出的 JSON 文件（可选，默认与输入同名） |
| `--no-fix` | 跳过 MIDI 修复（不推荐） |
| `--no-name` | 不包含音符名称（仅音高编号） |
| `--include-drums` | 包含打击乐轨道 |

### 输出 JSON 格式

```json
[
    {
        "pitch": 60,
        "start_ms": 1690,
        "duration_ms": 422,
        "note_name": "C4"
    },
    ...
]
```

### 示例

```bash
# 基本用法
python scribe_main.py my_song.mid

# 指定输出路径
python scribe_main.py my_song.mid output.json

# 不包含音符名称（节省空间）
python scribe_main.py my_song.mid --no-name
```

所有音符按 `start_ms` 排序，和弦/重叠音符会同时出现（同一时刻多个条目）。

---

这个工具完全独立，可以直接使用。等 C++ 项目重建后，再把生成的 JSON 作为数据源加载。😊