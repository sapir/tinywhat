from .utils import assert_eval


def test_add():
    assert_eval('(+ 1 2)', 3)
    assert_eval('(+ 5 10 3)', 18)


def test_sub():
    assert_eval('(- 5 1)', 4)
    assert_eval('(- 1 2)', -1)
    assert_eval('(- 1 10)', -9)


def test_mul():
    assert_eval('(* 5 1)', 5)
    assert_eval('(* 10 200)', 2000)
