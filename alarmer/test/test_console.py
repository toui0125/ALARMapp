from unittest import result
from alarmer.views import console

import pytest


def test_get_document_dir_path():
  result = console.get_document_dir_path()
  assert result == '/Users/kawaguchitoui/Downloads/remember/alarmer/documents'

def test1_find_document():
  result = console.find_document('hello.txt')
  assert result == '/Users/kawaguchitoui/Downloads/remember/alarmer/documents/hello.txt'

def test2_find_document():
  with pytest.raises(console.NoDocumentError):
    console.find_document('sample.txt')

@pytest.mark.skip(reason = 'I performed black-box and white-box testing')
def test_get_document():
  pass

@pytest.mark.skip(reason = 'I performed black-box and white-box testing')
def test_make_message_from_document():
  pass