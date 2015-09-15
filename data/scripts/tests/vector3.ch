def vector3Smoke()
{
    var v = Vector3();
    v := Vector3(1, 2, 3);

    var scalar = v.x;
    scalar = v.y;
    scalar = v.z;

    var unitX = Vector3_unitX();
    var unitY = Vector3_unitY();
    var unitZ = Vector3_unitZ();
    var zero = Vector3_zero();
    var unit = Vector3_unit();

    zero.isZero();
    unit.isUnit();

    var angle = Vector3_angle(unitX, unit);

    zero.clamp(unitX, unitY);

    var cross = Vector3_cross(unit, unitY);

    scalar = zero.distance(unit);
    scalar = zero.distanceSquared(unit);

    var dot = unitX.dot(unit);
    dot = Vector3_dot(unit, unitY);

    var len = cross.length();
    len = cross.lengthSquared();

    var normalized = cross.normalized();
    normalized.normalize();

    normalized.set(1, 2, 3);
    normalized.set(Vector3(4, 5, 6));

    var plus = unitX + unitY;
    var plusScalar = unitX + 10.0f;
    plus += unitZ;
    plusScalar += 100.0f;
    var minus = unitX - unitY;
    var minusScalar = unitX - 10.0f;
    minus -= unitZ;
    minus -= 200.0f;

    var mult = unitX * 10.0f;
    mult *= 20.0f;

    var div = unitX / 1.0f;
    div /= 1.0f;

    var less = unitX < unitY;
    var eq = unitX == unitY;
    var neq = unitX != unitY;
}

vector3Smoke();