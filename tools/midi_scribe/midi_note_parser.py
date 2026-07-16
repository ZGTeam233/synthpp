#
# Created by wsnzg6 on 2026/7/16.
# Copyright(c) 2026 ZGTeam233.
#

import pretty_midi
from pathlib import Path
from typing import Optional, TextIO
import json

class MidiNoteParser:
    """
    用于解析 MIDI 文件并提取音符音高、起止时间、时长的类。
    结果可导出为 TXT 文件。
    """

    def __init__(self, midi_path: str):
        """
        初始化解析器。

        Args:
            midi_path (str): MIDI 文件的路径。

        Raises:
            FileNotFoundError: 当 MIDI 文件不存在时抛出。
            pretty_midi.PrettyMIDIError: 当文件无法解析时抛出。
        """
        self.midi_path = Path(midi_path)
        if not self.midi_path.exists():
            raise FileNotFoundError(f"MIDI 文件不存在: {self.midi_path}")
        self.midi_data = pretty_midi.PrettyMIDI(str(self.midi_path))
        self.notes_info = []   # 存储所有音符信息的列表

    def parse(self, skip_drums: bool = True) -> list:
        """
        解析 MIDI 文件，提取所有音符的音高、起始时间、时长。

        Args:
            skip_drums (bool): 是否跳过打击乐轨道，默认为 True。

        Returns:
            list: 包含字典的列表，每个字典代表一个音符：
                  {
                    'track': 轨道索引,
                    'pitch': MIDI 音高编号,
                    'start': 起始时间 (秒),
                    'duration': 时长 (秒)
                  }
        """
        self.notes_info.clear()
        for track_idx, instrument in enumerate(self.midi_data.instruments):
            if skip_drums and instrument.is_drum:
                continue
            for note in instrument.notes:
                self.notes_info.append({
                    'track': track_idx,
                    'pitch': note.pitch,
                    'start': note.start,
                    'duration': note.end - note.start
                })
        return self.notes_info

    def write_to_txt(self, output_path: str, format_str: Optional[str] = None) -> None:
        """
        将解析出的音符信息写入 TXT 文件。

        Args:
            output_path (str): 输出文件路径。
            format_str (Optional[str]): 自定义每行的格式字符串，可使用变量：
                                        {track}, {pitch}, {start}, {duration}
                                        例如: "轨道:{track} 音高:{pitch} 开始:{start:.2f}s 时长:{duration:.2f}s"
                                        若为 None，则使用默认格式。

        Raises:
            RuntimeError: 如果尚未调用 parse() 或解析结果为空。
        """
        if not self.notes_info:
            raise RuntimeError("没有音符数据，请先调用 parse() 方法。")

        if format_str is None:
            format_str = "轨道:{track} | 音高:{pitch} | 开始:{start:.3f}s | 时长:{duration:.3f}s"

        with open(output_path, 'w', encoding='utf-8') as f:
            for note in self.notes_info:
                line = format_str.format(**note)
                f.write(line + '\n')

        print(f"成功写入 {len(self.notes_info)} 条音符信息到: {output_path}")

    def parse_and_save(self, output_path: str, skip_drums: bool = True, format_str: Optional[str] = None) -> None:
        """
        组合方法：先解析再保存。
        """
        self.parse(skip_drums=skip_drums)
        self.write_to_txt(output_path, format_str)

    def write_to_json(self, output_path: str, indent: int = 4, include_note_name: bool = False) -> None:
        """
        将解析出的音符信息写入 JSON 文件。

        Args:
            output_path (str): 输出 JSON 文件路径。
            indent (int): JSON 缩进空格数，默认 4（美化输出）。
            include_note_name (bool): 是否在每条记录中添加音符名称（如 'C5'），默认 False。

        Raises:
            RuntimeError: 如果尚未调用 parse() 或解析结果为空。
        """
        if not self.notes_info:
            raise RuntimeError("没有音符数据，请先调用 parse() 方法。")

        # 如果需要添加音符名称，复制数据并补充
        data_to_export = self.notes_info.copy()
        if include_note_name:
            for note in data_to_export:
                note['note_name'] = pretty_midi.note_number_to_name(note['pitch'])

        with open(output_path, 'w', encoding='utf-8') as f:
            json.dump(data_to_export, f, indent=indent, ensure_ascii=False)

        print(f"成功写入 {len(data_to_export)} 条音符信息到: {output_path}")

    def parse_and_save_json(self, output_path: str, skip_drums: bool = True, indent: int = 4, include_note_name: bool = False) -> None:
        self.parse(skip_drums=skip_drums)
        self.write_to_json(output_path, indent=indent, include_note_name=include_note_name)