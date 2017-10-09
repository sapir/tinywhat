from .utils import what_eval


def test_number():
    assert what_eval('5\n') == ' -> 5\n'
