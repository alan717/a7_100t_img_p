// "Iced Cake" by dr2 - 2018
// License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License

float PrSphDf (vec3 p, float r);
float PrCylDf (vec3 p, float r, float h);
float PrTorusDf (vec3 p, float ri, float rc);
float SmoothMin (float a, float b, float r);
float SmoothBump (float lo, float hi, float w, float x);
vec2 Rot2D (vec2 q, float a);
vec2 Rot2Cs (vec2 q, vec2 cs);
float Noisefv2 (vec2 p);
vec3 VaryNf (vec3 p, vec3 n, float f);

vec3 qHit;
float dstFar, tCur, cakRd, cakHt, cakSlc;
int idObj;
const float pi = 3.14159;

#define DMINQ(id) if (d < dMin) { dMin = d;  idObj = id;  qHit = q; }

float ObjDf (vec3 p)
{
  vec3 q, qq, qs, qr;
  vec2 cs, cs2;
  float dMin, d, r, dw1, dw2, db, c;
  dMin = dstFar;
  c = cos (pi / (2. * cakSlc));
  cs = vec2 (cos (pi / cakSlc), sin (pi / cakSlc));
  cs2 = vec2 (cos (2. * pi / cakSlc), sin (2. * pi / cakSlc));
  dw1 = SmoothBump (0.15, 0.6, 0.1, mod (0.1 * tCur, 1.));
  dw2 = SmoothBump (0.4, 0.85, 0.1, mod (0.1 * tCur, 1.));
  r = length (p.xz);
  qs = p;
  qs.xz = Rot2D (qs.xz, 2. * pi * (floor (cakSlc * ((r > 0.) ? atan (qs.z, - qs.x) / (2. * pi) :
     0.) + 0.5) / cakSlc));
  qr = p;
  qr.xz = Rot2Cs (qr.xz, cs);
  qr.xz = Rot2D (qr.xz, 2. * pi * (floor (cakSlc * ((r > 0.) ? atan (qr.z, - qr.x) / (2. * pi) :
     0.) + 0.5) / cakSlc));
  for (int k = 0; k < 3; k ++) {
    q = qs;
    qq = p;
    if (k == 0) {
      qq.xz = Rot2D (qq.xz, pi / cakSlc);
      db = - dot (vec2 (abs (qq.z), qq.x), cs2);
    } else {
      qq = p;
      if (k == 1) {
        db = max ((dw1 * cs.y - abs (qr.z)) * c, dot (vec2 (abs (qq.z), dw1 + qq.x), cs));
        q.x += dw1;
      } else {
        qq.xz = Rot2D (qq.xz, 2. * pi / cakSlc);
        db = max ((dw2 * cs.y - abs (qr.z)) * c, dot (vec2 (abs (qq.z), dw2 + qq.x), cs));
        q.x += dw2;
      }
    }
    d = PrCylDf (q.xzy, cakRd, cakHt);
    d = SmoothMin (d, length (vec2 (abs (abs (length (q.xz) - 0.47 * cakRd) - 0.33 * cakRd) -
       0.2 * cakRd + 0.05, q.y - cakHt)) - 0.05, 0.03);
    d = SmoothMin (d, PrTorusDf ((q + vec3 (0.8 * cakRd, - cakHt, 0.)).xzy, 0.05, 0.15 * cakRd), 0.03);
    d = SmoothMin (d, PrTorusDf (vec3 (q.xz, abs (q.y) - 0.33 * cakHt), 0.05 * (1. +
       0.5 * SmoothBump (-0.1, 0.1, 0.02, q.z / cakRd)), cakRd), 0.03);
    d = max (d, db);
    DMINQ (1);
    q.y -= cakHt;
    d = max (PrSphDf (q, 0.1), db);
    DMINQ (2);
    q.x -= -0.8 * cakRd;
    d = max (PrSphDf (q, 0.1), db);
    DMINQ (3);
  }
  q = p;
  q.y -= - cakHt - 0.051;
  d = PrCylDf (q.xzy, cakRd + 1.1, 0.05);
  DMINQ (4);
  return dMin;
}
     
float ObjRay (vec3 ro, vec3 rd)
{
  float dHit, d;
  dHit = 0.;
  for (int j = 0; j < 150; j ++) {
    d = ObjDf (ro + rd * dHit);
    if (d < 0.0005 || dHit > dstFar) break;
    dHit += d;
  }
  return dHit;
}

vec3 ObjNf (vec3 p)
{
  vec4 v;
  vec2 e = vec2 (0.0001, -0.0001);
  v = vec4 (ObjDf (p + e.xxx), ObjDf (p + e.xyy), ObjDf (p + e.yxy), ObjDf (p + e.yyx));
  return normalize (vec3 (v.x - v.y - v.z - v.w) + 2. * v.yzw);
}

vec3 ShowScene (vec3 ro, vec3 rd)
{
  vec3 ltPos[4], ltDir, col, vn;
  vec2 s;
  float dstObj, spec, r, a, t, dfSum, spSum, at;
  bool isRefl;
  for (int k = 0; k < 3; k ++) {
    ltPos[k] = 7. * vec3 (0., 1., 1.);
    ltPos[k].xz = Rot2D (ltPos[k].xz, float (k) * 2. * pi / 3. -0.1 * pi * tCur);
  }
  ltPos[3] = 7. * vec3 (1., 1., 0.);
  ltPos[3].xz = Rot2D (ltPos[3].xz, 0.1 * pi * tCur);
  cakRd = 2.;
  cakHt = 0.5;
  cakSlc = 13.;
  dstObj = ObjRay (ro, rd);
  isRefl = false;
  if (dstObj < dstFar && idObj == 4) {
    if (length (qHit.xz) < 3.05) {
      isRefl = true;
      ro += dstObj * rd;
      vn = ObjNf (ro);
      vn = VaryNf (32. * qHit, vn, 0.5);
      rd = reflect (rd, vn);
      ro += 0.01 * rd;
      dstObj = ObjRay (ro, rd);
    }
  }
  if (dstObj < dstFar) {
    ro += dstObj * rd;
    vn = ObjNf (ro);
    if (idObj == 1) {
      r = length (qHit.xz);
      if (abs (qHit.y) < cakHt - 0.05 && r < cakRd - 0.1) {
        t = 3. * qHit.y / (2. * cakHt) + 0.1 * sin (4. * pi * r);
        col = mix (((floor (t + 0.5) == 0.) ? vec3 (0.6, 0.3, 0.1) : vec3 (0.9, 0.7, 0.1)),
           vec3 (0.6, 0., 0.), SmoothBump (-0.05, 0.05, 0.01, mod (t, 1.) - 0.5)) *
           (1. - 0.1 * Noisefv2 (64. * vec2 (r, qHit.y)));
        spec = 0.05;
        vn = VaryNf (64. * vec3 (r, qHit.yz), vn, 1.);
      } else {
        a = (r > 0.) ? atan (qHit.z, - qHit.x) / (2. * pi) : 0.;
        s = vec2 (8. * abs (r - 0.5 * cakRd + 0.05), mod (4. * cakSlc * a, 1.) - 0.5);
        t = length (s);
        col = vec3 (0.4, 0.2, 0.);
        if (qHit.y > cakHt - 0.001 && t < 0.48) {
          col = mix (vec3 (0.3, 0.1, 0.), col, smoothstep (0.2, 0.25, t));
          vn.xz = 0.2 * normalize (s) * SmoothBump (0.25, 0.43, 0.05, t);
          vn.y = sqrt (1. - dot (vn.xz, vn.xz));
        }
        col = mix (col, vec3 (0.9, 0.7, 0.5), smoothstep (0.005, 0.015, qHit.y - cakHt));
        spec = 0.2;
        if (abs (qHit.y) < cakHt && abs (r - cakRd) < 0.005)
           vn.xz = Rot2D (vn.xz, -0.2 * sin (pi * (mod (16. * cakSlc * a, 1.) - 0.5)));
      }
    } else if (idObj == 2) {
      col = mix (vec3 (0.9, 0.3, 0.2), vec3 (0.3, 0.9, 0.2), step (length (qHit), 0.07));
      spec = 0.1;
    } else if (idObj == 3) {
      col = vec3 (0.9, 0.9, 0.95);
      spec = 0.4;
    } else if (idObj == 4) {
      col = vec3 (0.9, 0.9, 0.7);
      spec = 0.2;
    }
    dfSum = 0.;
    spSum = 0.;
    for (int k = 0; k < 4; k ++) {
      ltDir = normalize (ltPos[k]);
      at = smoothstep (0.5, 0.6, dot (normalize (ltPos[k] - ro), ltDir));
      dfSum += at * max (dot (vn, ltDir), 0.);
      spSum += at * pow (max (dot (normalize (ltDir - rd), vn), 0.), 32.);
    }
    col = col * (0.1 + 0.35 * dfSum);
    if (! isRefl) col += 0.6 * spec * spSum;
    if (isRefl) col = mix (col, vec3 (0.8), 0.05);
  } else {
    col = vec3 (0.6, 0.6, 1.) * (0.05 + 0.245 * (rd.y + 1.) * (rd.y + 1.));
  }
  return clamp (col, 0., 1.);
}

void mainImage (out vec4 fragColor, in vec2 fragCoord)
{
  mat3 vuMat;
  vec4 mPtr;
  vec3 ro, rd;
  vec2 canvas, uv, ori, ca, sa;
  float el, az, zmFac;
  canvas = iResolution.xy;
  uv = 3. * fragCoord.xy / canvas - 1.;
  uv.x *= canvas.x / canvas.y;
  tCur = iTime;
  mPtr = iMouse;
  mPtr.xy = mPtr.xy / canvas - 0.5;
  az = (15./26.) * pi;
  el = -0.18 * pi;
  if (mPtr.z > 0.) {
    az += 3. * pi * mPtr.x;
    el += pi * mPtr.y;
  } else {
    az += 0.03 * pi * tCur;
    el -= 0.12 * pi * sin (0.05 * pi * tCur);
  }
  el = clamp (el, -0.4 * pi, -0.05 * pi);
  ori = vec2 (el, az);
  ca = cos (ori);
  sa = sin (ori);
  vuMat = mat3 (ca.y, 0., - sa.y, 0., 1., 0., sa.y, 0., ca.y) *
          mat3 (1., 0., 0., 0., ca.x, - sa.x, 0., sa.x, ca.x);
  ro = vuMat * vec3 (0., 0., -15.);
  zmFac = 7. - 2. * abs (el); //0.8 * lookU.z;
  rd = vuMat * normalize (vec3 (uv, zmFac));
  dstFar = 30.;
  fragColor = vec4 (ShowScene (ro, rd), 1.);
}

float PrSphDf (vec3 p, float r)
{
  return length (p) - r;
}

float PrCylDf (vec3 p, float r, float h)
{
  return max (length (p.xy) - r, abs (p.z) - h);
}

float PrTorusDf (vec3 p, float ri, float rc)
{
  return length (vec2 (length (p.xy) - rc, p.z)) - ri;
}

float SmoothMin (float a, float b, float r)
{
  float h;
  h = clamp (0.5 + 0.5 * (b - a) / r, 0., 1.);
  return mix (b, a, h) - r * h * (1. - h);
}

float SmoothBump (float lo, float hi, float w, float x)
{
  return (1. - smoothstep (hi - w, hi + w, x)) * smoothstep (lo - w, lo + w, x);
}

vec2 Rot2D (vec2 q, float a)
{
  return q * cos (a) + q.yx * sin (a) * vec2 (-1., 1.);
}

vec2 Rot2Cs (vec2 q, vec2 cs)
{
  return vec2 (dot (q, vec2 (cs.x, - cs.y)), dot (q.yx, cs));
}

const float cHashM = 43758.54;

vec2 Hashv2v2 (vec2 p)
{
  vec2 cHashVA2 = vec2 (37., 39.);
  return fract (sin (vec2 (dot (p, cHashVA2), dot (p + vec2 (1., 0.), cHashVA2))) * cHashM);
}

float Noisefv2 (vec2 p)
{
  vec2 t, ip, fp;
  ip = floor (p);  
  fp = fract (p);
  fp = fp * fp * (3. - 2. * fp);
  t = mix (Hashv2v2 (ip), Hashv2v2 (ip + vec2 (0., 1.)), fp.y);
  return mix (t.x, t.y, fp.x);
}

float Fbmn (vec3 p, vec3 n)
{
  vec3 s;
  float a;
  s = vec3 (0.);
  a = 1.;
  for (int j = 0; j < 5; j ++) {
    s += a * vec3 (Noisefv2 (p.yz), Noisefv2 (p.zx), Noisefv2 (p.xy));
    a *= 0.5;
    p *= 2.;
  }
  return dot (s, abs (n));
}

vec3 VaryNf (vec3 p, vec3 n, float f)
{
  vec3 g;
  vec2 e = vec2 (0.1, 0.);
  g = vec3 (Fbmn (p + e.xyy, n), Fbmn (p + e.yxy, n), Fbmn (p + e.yyx, n)) - Fbmn (p, n);
  return normalize (n + f * (g - n * dot (n, g)));
}
