#!/usr/bin/python3
import os

def create_tag(name, desc):
    print(f'Create tag {name}: {desc}')
    os.system(f'git tag -a {name} -m "{desc}"')


def create_diff(name):
    os.system(f'git show {name} > docs/versions/{name}.diff')


def log():
    os.system('git log > CHANGELOG.md')


name = input("Tag name: ")
message = input("Tag message: ")
create_tag(name, message)
create_diff(name)
log()

print("Done!")
