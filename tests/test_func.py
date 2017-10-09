from .utils import assert_eval


def test_simple_func():
    assert_eval('(def @a $a 8) (@a)', 1, 8)


def test_simple_func_args():
    assert_eval(
        '(def @a $a $a)'
        '(@a 1)'
        '(@a 2)'
        '(@a 5)',
        1,
        1,
        2,
        5)


def test_func_args_overwrite_globals():
    assert_eval(
        '(def @a $a 3)'
        '(set $a 10)'
        '$a'
        '(@a 8)'
        '$a',
        1,
        10,
        10,
        3,
        8,
        )


def test_func_args_with_offset():
    assert_eval(
        '(def @a $d (+ $d $i))'
        '(def @b $i (+ $i $j))'
        '(@a 1 2 3)'
        '(@b 8 9 10)'
        '$a\n$b\n$c\n$d\n$e\n$i\n$j\n$k\n',
        1, 1,
        4,
        17,
        0, 0, 0, 1, 2, 8, 9, 10,
        )


def test_no_args():
    assert_eval(
        '(def @a (+ 0 5))'
        '(@a)',
        1,
        5,
    )


def test_nested_calls():
    assert_eval(
        '(def @a $a (+ $a $b))'
        '(def @b (+ (@a 1 2) (@a 3 4)))'
        '(@b)',
        1,
        1,
        1 + 2 + 3 + 4,
    )
