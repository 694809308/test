/*
 * bio.c
 *
 *  Created on: 2016年3月22日
 *      Author: Admin
 */

void bio_copy_data(struct bio *dst, struct bio *src)
{
	struct bio_vec *src_bv, *dst_bv;
	unsigned src_offset, dst_offset, bytes;
	//+ struct bvec_iter src_iter, dst_iter;
	//+ struct bio_vec src_bv, dst_bv;
	void *src_p, *dst_p;
	//+ unsigned bytes;

	src_bv = __bio_iovec(src);	//bio_iovec(bio)
	dst_bv = __bio_iovec(dst);

	src_offset = src_bv->bv_offset;
	dst_offset = dst_bv->bv_offset;
	//+ src_iter = src->bi_iter;
	//+ dst_iter = dst->bi_iter;

	while (1) {

		//该bio_vec中的bv_len为0，没有数据
		if (src_offset == src_bv->bv_offset + src_bv->bv_len) {
			//下一个bio_vec
			src_bv++;

			//该bio_vec为最后一个
			if (src_bv == bio_iovec_idx(src, src->bi_vcnt)) {
				//
				src = src->bi_next;
				if (!src)
					break;

				src_bv = __bio_iovec(src);
			}
			//+ if (!src_iter.bi_size) {
			//+ src = src->bi_next;
			//+ if (!src)
			//+ break;

			src_offset = src_bv->bv_offset;
			//+ src_iter = src->bi_iter;
		}

		if (dst_offset == dst_bv->bv_offset + dst_bv->bv_len) {
			dst_bv++;
			if (dst_bv == bio_iovec_idx(dst, dst->bi_vcnt)) {
				dst = dst->bi_next;
				if (!dst)
					break;

				dst_bv = __bio_iovec(dst);
		}
		//+ if (!dst_iter.bi_size) {
		//+ dst = dst->bi_next;
		//+ if (!dst)
		//+ break;

		dst_offset = dst_bv->bv_offset;
		//+ dst_iter = dst->bi_iter;
		}

		//
		bytes = min(dst_bv->bv_offset + dst_bv->bv_len - dst_offset,
					src_bv->bv_offset + src_bv->bv_len - src_offset);
		//+ src_bv = bio_iovec_iter(src, src_iter);
		//+ dst_bv = bio_iovec_iter(dst, dst_iter);
		//+
		//+ bytes = min(src_bv.bv_len, dst_bv.bv_len);

		src_p = kmap_atomic(src_bv->bv_page);
		dst_p = kmap_atomic(dst_bv->bv_page);
		//+ src_p = kmap_atomic(src_bv.bv_page);
		//+ dst_p = kmap_atomic(dst_bv.bv_page);

		memcpy(dst_p + dst_bv->bv_offset,
				src_p + src_bv->bv_offset,
				//+ memcpy(dst_p + dst_bv.bv_offset,
				//+ src_p + src_bv.bv_offset,
				bytes);

		kunmap_atomic(dst_p);
		kunmap_atomic(src_p);

		src_offset += bytes;
		dst_offset += bytes;
		//+ bio_advance_iter(src, &src_iter, bytes);
		//+ bio_advance_iter(dst, &dst_iter, bytes);
	}
}
EXPORT_SYMBOL(bio_copy_data);
