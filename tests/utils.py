from tempfile import TemporaryFile
from io import BytesIO
from subprocess import check_output


def what_eval(inp):
    with TemporaryFile() as tf:
        tf.write(inp)
        tf.seek(0)
        output_str = check_output('./test', stdin=tf)
        return output_str


def assert_eval(inp, out):
    assert what_eval(inp + '\n') == ' -> {}\n'.format(out)
