import re
from tempfile import TemporaryFile
from io import BytesIO
from subprocess import check_output


def what_eval(inp, parse=True):
    with TemporaryFile() as tf:
        tf.write(inp)
        tf.seek(0)

        output_str = check_output('./test', stdin=tf)

        if not parse:
            return output_str

        return re.findall(r'(?m)(?<= -> )[-0-9]+|^(?! -> ).+', output_str)


def assert_eval(inp, *outs):
    outs = map(str, outs)
    actual_outs = what_eval(inp + '\n')
    assert outs == actual_outs, (
        "expected: {}; actual: {}".format(outs, actual_outs))
