/*
 * EAP peer: EAP-TLS/PEAP/TTLS/FAST common functions
 * Copyright (c) 2004-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef EAP_TLS_COMMON_H
#define EAP_TLS_COMMON_H

/**
 * struct eap_ssl_data - TLS data for EAP methods
 */
struct eap_ssl_data {
	/**
	 * conn - TLS connection context data from tls_connection_init()
	 */
	struct tls_connection *conn;

	/**
	 * tls_out - TLS message to be sent out in fragments
	 */
	u8 *tls_out;

	/**
	 * tls_out_len - Total length of the outgoing TLS message
	 */
	size_t tls_out_len;

	/**
	 * tls_out_pos - The current position in the outgoing TLS message
	 */
	size_t tls_out_pos;

	/**
	 * tls_out_limit - Maximum fragment size for outgoing TLS messages
	 */
	size_t tls_out_limit;

	/**
	 * tls_in - Received TLS message buffer for re-assembly
	 */
	u8 *tls_in;

	/**
	 * tls_in_len - Number of bytes of the received TLS message in tls_in
	 */
	size_t tls_in_len;

	/**
	 * tls_in_left - Number of remaining bytes in the incoming TLS message
	 */
	size_t tls_in_left;

	/**
	 * tls_in_total - Total number of bytes in the incoming TLS message
	 */
	size_t tls_in_total;

	/**
	 * phase2 - Whether this TLS connection is used in EAP phase 2 (tunnel)
	 */
	int phase2;

	/**
	 * include_tls_length - Whether the TLS length field is included even
	 * if the TLS data is not fragmented
	 */
	int include_tls_length;

	/**
	 * tls_ia - Whether TLS/IA is enabled for this TLS connection
	 */
	int tls_ia;

	/**
	 * eap - Pointer to EAP state machine allocated with eap_peer_sm_init()
	 */
	struct eap_sm *eap;
};


/* EAP TLS Flags */
#define EAP_TLS_FLAGS_LENGTH_INCLUDED 0x80
#define EAP_TLS_FLAGS_MORE_FRAGMENTS 0x40
#define EAP_TLS_FLAGS_START 0x20
#define EAP_PEAP_VERSION_MASK 0x07

 /* could be up to 128 bytes, but only the first 64 bytes are used */
#define EAP_TLS_KEY_LEN 64


int eap_peer_tls_ssl_init(struct eap_sm *sm, struct eap_ssl_data *data,
			  struct wpa_ssid *config);
void eap_peer_tls_ssl_deinit(struct eap_sm *sm, struct eap_ssl_data *data);
u8 * eap_peer_tls_derive_key(struct eap_sm *sm, struct eap_ssl_data *data,
			     const char *label, size_t len);
const u8 * eap_peer_tls_data_reassemble(
	struct eap_ssl_data *data, const u8 *in_data, size_t in_len,
	size_t *out_len, int *need_more_input);
int eap_peer_tls_process_helper(struct eap_sm *sm, struct eap_ssl_data *data,
				EapType eap_type, int peap_version,
				u8 id, const u8 *in_data, size_t in_len,
				u8 **out_data, size_t *out_len);
u8 * eap_peer_tls_build_ack(struct eap_ssl_data *data, size_t *respDataLen,
			    u8 id, EapType eap_type, int peap_version);
int eap_peer_tls_reauth_init(struct eap_sm *sm, struct eap_ssl_data *data);
int eap_peer_tls_status(struct eap_sm *sm, struct eap_ssl_data *data,
			char *buf, size_t buflen, int verbose);
const u8 * eap_peer_tls_process_init(struct eap_sm *sm,
				     struct eap_ssl_data *data,
				     EapType eap_type,
				     struct eap_method_ret *ret,
				     const u8 *reqData, size_t reqDataLen,
				     size_t *len, u8 *flags);
void eap_peer_tls_reset_input(struct eap_ssl_data *data);
void eap_peer_tls_reset_output(struct eap_ssl_data *data);
int eap_peer_tls_decrypt(struct eap_sm *sm, struct eap_ssl_data *data,
			 const u8 *in_data, size_t in_len,
			 u8 **in_decrypted, size_t *len_decrypted);
int eap_peer_tls_encrypt(struct eap_sm *sm, struct eap_ssl_data *data,
			 EapType eap_type, int peap_version, u8 id,
			 const u8 *in_data, size_t in_len,
			 u8 **out_data, size_t *out_len);
int eap_peer_select_phase2_methods(struct wpa_ssid *config, const char *prefix,
				   struct eap_method_type **types,
				   size_t *num_types);
int eap_peer_tls_phase2_nak(struct eap_method_type *types, size_t num_types,
			    struct eap_hdr *hdr, u8 **resp, size_t *resp_len);

#endif /* EAP_TLS_COMMON_H */
