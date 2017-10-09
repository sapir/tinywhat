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


def test_div():
    assert_eval('(/ 1 2)', 0)
    assert_eval('(/ 2 1)', 2)
    assert_eval('(/ 8 2)', 4)
    assert_eval('(/ 9 2)', 4)
    assert_eval('(/ 10 5)', 2)


def test_mod():
    assert_eval('(% 1 2)', 1)
    assert_eval('(% 2 1)', 0)
    assert_eval('(% 7 3)', 1)
    assert_eval('(% 8 3)', 2)
    assert_eval('(% 9 3)', 0)
    assert_eval('(% 10 5)', 0)


def test_eq():
    assert_eval('(= 1 2)', 0)
    assert_eval('(= 2 2)', 1)
