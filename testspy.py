import unittest
import base64
import subprocess
import os
import random
import string

ASCII85_EXEC_PATH = './ascii85'

def encode_to_ascii85_with_exec(text: str):
    result = subprocess.run(
        [ASCII85_EXEC_PATH],
        input=text,
        capture_output=True,
        text=True
    )
    return {
        "content": result.stdout.strip(),
        "code": result.returncode,
        "error": result.stderr.strip()
    }

def decode_from_ascii85_with_exec(text: str):
    result = subprocess.run(
        [ASCII85_EXEC_PATH, "-d"],
        input=text,
        capture_output=True,
        text=True
    )
    return {
        "content": result.stdout.strip(),
        "code": result.returncode,
        "error": result.stderr.strip()
    }

class TestASCII85Exec(unittest.TestCase):
    
    @classmethod
    def setUpClass(cls):
        if not os.path.exists(ASCII85_EXEC_PATH):
            raise FileNotFoundError(f"Executable file {ASCII85_EXEC_PATH} not found. Please run make to build.")
            
    def test_encoding(self):
        test_cases = ["Hello", "98765", "Hello, World!", "&*()_+="]
        for test_string in test_cases:
            with self.subTest(text=test_string):
                expected_ascii85 = base64.a85encode(test_string.encode("utf8")).decode('ascii')
                result = encode_to_ascii85_with_exec(test_string)
                self.assertEqual(result["code"], 0)
                self.assertEqual(result["content"], expected_ascii85)

    def test_decoding_positive(self):
        test_original = ["Hello", "98765", "Hello, World!", "&*()_+="]
        test_cases = [base64.a85encode(x.encode("utf-8")).decode('ascii') for x in test_original]
        for original, encoded in zip(test_original, test_cases):
            with self.subTest(text=encoded):
                result = decode_from_ascii85_with_exec(encoded)
                self.assertEqual(result["code"], 0)
                self.assertEqual(result["content"], original)

    def test_decoding_negative(self):
        test_failures = [
            ("invalid~ascii85", "Invalid character"),
            ("123z456", "Invalid 'z'")
        ]

        for test_input, expected_error_substring in test_failures:
            with self.subTest(text=test_input):
                result = decode_from_ascii85_with_exec(test_input)
                self.assertNotEqual(result["code"], 0, f"Input '{test_input}' should fail with non-zero exit code")
                
                output = result["error"] or result["content"]
                self.assertIn(
                    expected_error_substring,
                    output,
                    f"Expected error containing '{expected_error_substring}' for input '{test_input}', got: {output}"
                )

    def test_random_valid(self):
        for _ in range(5):
            length = random.randint(5, 20)
            text = ''.join(random.choices(string.ascii_letters + string.digits, k=length))
            with self.subTest(text=text):
                enc_result = encode_to_ascii85_with_exec(text)
                self.assertEqual(enc_result["code"], 0)

                dec_result = decode_from_ascii85_with_exec(enc_result["content"])
                self.assertEqual(dec_result["code"], 0)
                self.assertEqual(dec_result["content"], text)

if __name__ == '__main__':
    unittest.main()
