from alarmer.views import console

import pytest
import termcolor


def test_get_document_dir_path():
  result = console.get_document_dir_path()
  assert result == '/Users/kawaguchitoui/Downloads/remember/alarmer/documents'

def test1_find_document():
  result = console.find_document('hello.txt')
  assert result == '/Users/kawaguchitoui/Downloads/remember/alarmer/documents/hello.txt'

def test2_find_document():
  with pytest.raises(console.NoDocumentError):
    console.find_document('sample.txt')

@pytest.mark.skip(reason = 'Next test is enough')
def test_get_document():
  pass

def test_make_message_from_document():
  content = '='*60 + '\n12:12です。\nアラームを止めるには、controlとcを押してください。\n' + '='*60 + '\n'
  contents = termcolor.colored(content, 'magenta')
  result = console.make_message_from_document('hello.txt', 'magenta', sub_now = '12:12')
  assert result == contents