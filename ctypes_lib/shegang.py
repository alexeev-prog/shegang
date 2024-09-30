#!/usr/bin/python3
"""
 * @title SheGang Linux Shell | Python module
 * @filename colors.c
 * @brief Module for interactions with shegang sources in C with ctypes.
 * @authors Alexeev Bronislav
 * @license MIT License
"""
# TODO: integrate binary components
from pathlib import Path
import sys
import os
import ctypes


class LibShegang:
	"""
	LibShegang class
	"""
	def __init__(self, filepath: str):
		"""
		Initialize library
	
		:param filepath: Filepath to .so file
		:type: str
		"""
		self.filepath = Path(filepath)

		if not self.filepath.exists():
			raise FileNotFoundError(f"Library at path {self.filepath} is not exists")
		else:
			self.filepath = Path(os.path.abspath(os.path.join(os.path.dirname(__file__), filepath)))

		self.cdll = ctypes.CDLL(self.filepath)

	def launch_shell(self):
		"""
		Run shell
		"""
		print(f'\nPython implementation of shegang ({self.filepath})\n')
		self.cdll.main()


def main():
	"""
	Main function
	"""
	libshegang = LibShegang("bin/libshegang.so")
	libshegang.launch_shell()


if __name__ == '__main__':
	main()
