import unittest
import base64
import subprocess
import os

ASCII85_EXEC_PATH = './ascii85'

def encode_to_ascii85_with_exec(text: str):
    result = subprocess.run([ASCII85_EXEC_PATH], input=text, capture_output=True, text=True)
    return {
        "content": result.stdout.strip(),
        "code": result.returncode,
    }

def decode_from_ascii85_with_exec(text: str):
    result = subprocess.run([ASCII85_EXEC_PATH, "-d"], input=text, capture_output=True, text=True)
    return {
        "content": result.stdout.strip(),
        "code": result.returncode,
    }

class TestASCII85Exec(unittest.TestCase):
    
    @classmethod
    def setUpClass(cls):
        if not os.path.exists(ASCII85_EXEC_PATH):
            raise FileNotFoundError(f"Executable file {ASCII85_EXEC_PATH} not found. Please run make to build.")
            
def test_encoding(self):
    test_cases = ["Hello", "Разработка", "98765", "Hello, World!", "&*()_+="]
    for test_string in test_cases:
        with self.subTest(text=test_string):
            expected_ascii85 = base64.a85encode(test_string.encode("utf8")).decode('ascii')
            result = encode_to_ascii85_with_exec(test_string)
            self.assertEqual(result["code"], 0)
            self.assertEqual(result["content"], expected_ascii85)

def test_decoding_positive(self):
    test_original = ["Hello", "Разработка", "98765", "Hello, World!", "&*()_+="]
    test_cases = [base64.a85encode(x.encode("utf-8")).decode('ascii') for x in test_original]
    for original, encoded in zip(test_original, test_cases):
        with self.subTest(text=encoded):
            result = decode_from_ascii85_with_exec(encoded)
            self.assertEqual(result["code"], 0)
            self.assertEqual(result["content"], original)
            
def test_decoding_negative(self):
    test_failures = ["", "Привет", "invalid~ascii85", "123z456"]

    for test_failure in test_failures:
        with self.subTest(text=test_failure):
            result = decode_from_ascii85_with_exec(test_failure)
            self.assertEqual(result["code"], 1) 
            self.assertNotEqual(result["content"], "", f"Expected error message for input '{test_failure}'")

           
            expected_errors = ["Invalid", "Empty", "Incomplete", "Error"]
            self.assertTrue(
                any(err in result["content"] for err in expected_errors),
                f"Unexpected error message for input '{test_failure}': {result['content']}"
            )

if __name__ == '__main__':
    unittest.main()
