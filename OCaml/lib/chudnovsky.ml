open Z

let constant_a = of_int 13591409
let constant_b = of_int 545140134
let constant_c3 = of_string "10939058860032000"

let rec compute_bs n1 n2 =
  if equal (sub n2 n1) one then
    if equal n1 zero then
      (one, one, constant_a)
    else
      let p =
        mul
          (mul (sub (mul (of_int 6) n1) (of_int 5))
               (sub (mul (of_int 2) n1) one))
          (sub (mul (of_int 6) n1) one)
      in
      let q =
        mul (pow n1 3) constant_c3
      in
      let tmp =
        add (mul constant_b n1) constant_a
      in
      let t =
        mul p tmp
      in
      let t =
        if equal (erem n1 (of_int 2)) one then neg t else t
      in
      (p, q, t)

  else
    let m = div (add n1 n2) (of_int 2) in

    let (p1, q1, t1) = compute_bs n1 m in
    let (p2, q2, t2) = compute_bs m n2 in

    let p = mul p1 p2 in
    let q = mul q1 q2 in

    let temp1 = mul t1 q2 in
    let temp2 = mul p1 t2 in
    let t = add temp1 temp2 in

    (p, q, t)

let compute_pi digits =
  let terms = add (div (of_int digits) (of_int 14)) one in
  let (_, q, t) = compute_bs zero terms in
  let d = of_int 426880 in
  let e = of_int 10005 in
  let exp = Int.mul digits 2 in
  let scaled_e = mul e (pow (of_int 10) exp) in
  let sqrt_e = sqrt scaled_e in
  let numerator = mul (mul d sqrt_e) q in
  let denominator = t in
  let pi = div numerator denominator in
  let s = to_string pi in
  let len = String.length s in
  if len <= digits then
    "0." ^ (String.make (Int.sub digits len) '0') ^ s
  else
    let dot_pos = Int.sub len digits in
    (String.sub s 0 dot_pos) ^ "." ^ (String.sub s dot_pos digits)
