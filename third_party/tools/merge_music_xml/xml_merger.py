#!/usr/bin/env python3

#
# Created by wsnzg6 on 2026/7/17.
# Copyright(c) 2026 ZGTeam233.
#

from music21 import converter, stream
from typing import List, Optional, Tuple
import copy
import os

class ScorePartMapper:
    """声部映射器：负责处理两页之间声部的对应关系"""

    @staticmethod
    def match_by_index(parts_a: List, parts_b: List) -> List[Tuple[int, int]]:
        """
        默认策略：按索引匹配（A页的第1行对应B页的第1行）
        如果后续想按乐器名称匹配，可以在这里扩展新策略
        """
        if len(parts_a) != len(parts_b):
            print(f"⚠️ 声部数量不匹配: {len(parts_a)} vs {len(parts_b)}，将按最小数量截断匹配")
        match_pairs = []
        for idx in range(min(len(parts_a), len(parts_b))):
            match_pairs.append((idx, idx))
        return match_pairs


class MusicXMLMerger:
    """
    乐谱合并引擎
    支持：多页合并、声部映射策略、元数据继承
    """

    def __init__(self, mapper: Optional[ScorePartMapper] = None):
        self.mapper = mapper if mapper else ScorePartMapper()
        self._merged_score: Optional[stream.Score] = None

    def add_page(self, file_path: str) -> 'MusicXMLMerger':
        """
        链式调用：添加一页乐谱（支持传入多页）
        内部维护一个待合并的乐谱列表
        """
        if not hasattr(self, '_pages'):
            self._pages = []
        try:
            score = converter.parse(file_path)
            self._pages.append(score)
            print(f"成功加载: {os.path.basename(file_path)} (声部数: {len(score.parts)})")
        except Exception as e:
            raise RuntimeError(f"解析文件失败 {file_path}: {str(e)}")
        return self

    def execute(self, output_path: str = "merged_full_score.musicxml") -> None:
        """执行合并逻辑"""
        if len(self._pages) < 2:
            raise ValueError("至少需要2页才能合并！")

        # 以第一页为基准创建新乐谱
        base_score = self._pages[0]
        self._merged_score = stream.Score()

        # 深拷贝第一页的元数据（标题、作者、拍号等）
        if base_score.metadata:
            self._merged_score.metadata = copy.deepcopy(base_score.metadata)

        # 获取声部匹配方案
        match_pairs = self.mapper.match_by_index(base_score.parts, self._pages[1].parts)

        # 遍历基准页的每个声部
        for base_idx, target_idx in match_pairs:
            # 取出基准页的声部作为容器
            base_part = base_score.parts[base_idx]
            # 创建一个新的声部对象，避免直接修改原数据导致副作用
            new_part = copy.deepcopy(base_part)

            # 取出第二页对应的声部
            second_part = self._pages[1].parts[target_idx]

            # 获取第二页该声部的所有小节（Measure）
            measures_to_append = second_part.getElementsByClass('Measure')
            for measure in measures_to_append:
                # 深拷贝每个小节的音符、休止符、和弦等
                new_part.append(copy.deepcopy(measure))

            # 将合并好的声部加入总谱
            self._merged_score.append(new_part)
            print(f"声部 {base_idx+1} 合并完成 (接入了第 {target_idx+1} 页的声部)")

        # 写入文件
        self._merged_score.write('musicxml', fp=output_path)
        print(f"总谱合并完成！保存至: {output_path}")

    def get_result(self) -> Optional[stream.Score]:
        """获取合并后的 stream 对象，便于后续在代码中继续处理"""
        return self._merged_score


class AdvancedScoreMerger(MusicXMLMerger):
    """高级合并器：支持自定义声部匹配规则（扩展演示）"""

    def custom_match_by_instrument_name(self, page_a_path: str, page_b_path: str):
        """
        未来的扩展点：如果AI识别时声部顺序乱了，可以根据乐器名称（如 'Piano'、'Violin'）来匹配
        这里只是占位，展示OOP的继承优势
        """
        print("使用高级乐器名称匹配策略...")
        # 具体实现略，可根据实际需求补充
        pass