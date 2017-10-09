from .utils import assert_eval


def test_number():
    assert_eval('5', 5)
    assert_eval('100', 100)
    assert_eval('4123', 4123)
