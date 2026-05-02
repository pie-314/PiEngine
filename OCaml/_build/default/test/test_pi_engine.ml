open Pi_engine_lib.Chudnovsky

let test_pi digits expected =
  let result = compute_pi digits in
  if result = expected then
    Printf.printf "Test %d digits: PASS\n" digits
  else
    Printf.printf "Test %d digits: FAIL (expected %s, got %s)\n" digits expected result

let () =
  test_pi 10 "3.1415926535";
  test_pi 50 "3.14159265358979323846264338327950288419716939937510"
