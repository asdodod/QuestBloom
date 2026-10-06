import fs from 'node:fs';
import assert from 'node:assert/strict';

const { instance } = await WebAssembly.instantiate(fs.readFileSync(process.argv[2]));
const native = instance.exports;
// Reference the game's RenderBloom level calculation and its upsample loop.
function destinationWrites(width, height, radius) {
    const levels = Math.max(1, Math.min(16,
        Math.floor(Math.log2(Math.max(width, height)) + Math.min(radius, 10) - 10)));
    return levels >= 2 ? 1 : 0;
}
assert.equal(destinationWrites(512, 512, 1), 0, 'Old size 1 leaves the destination unwritten');
assert.equal(destinationWrites(512, 512, 2), 0, 'Old size 2 leaves the destination unwritten');
assert.equal(destinationWrites(512, 512, 2.5), 0, 'Reported size 2.5 retains the stale bloom frame');
assert.equal(native.BoundSize(1), native.MinimumSize());
assert.equal(native.BoundSize(2.5), native.MinimumSize());
assert.equal(native.BoundSize(4), 4);
assert.equal(native.BoundSize(100), native.MaximumSize());
assert.equal(native.BoundSize(NaN), 4);
let cases = 0;
for (const width of [512, 768, 1024])
    for (const aspect of [0.25, 0.5, 1, 1.5, 2])
        for (let size = -2; size <= 10; size += 0.5) {
            assert.equal(destinationWrites(width, Math.floor(width * aspect), native.BoundSize(size)), 1);
            cases++;
        }
console.log(`BLOOM_SIZE_REGRESSION_OK: ${cases} cases write the destination; old size 2.5 reproduced`);
