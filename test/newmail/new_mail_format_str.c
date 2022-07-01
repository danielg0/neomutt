/**
 * @file
 * Test code for new_mail_format_str()
 *
 * @authors
 * Copyright (C) 2022 Michal Siedlaczek <michal@siedlaczek.me>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mutt/string2.h"
#define TEST_NO_MAIN
#include "config.h"
#include "acutest.h"
#include "email/lib.h"
#include "core/lib.h"
#include "newmail/lib.h"

void test_new_mail_format_str(void)
{
  // const char *new_mail_format_str(char *buf, size_t buflen, size_t col, int cols,
  //                                 char op, const char *src, const char *prec,
  //                                 const char *if_str, const char *else_str,
  //                                 intptr_t data, MuttFormatFlags flags);

  char buf[64];
  size_t col = 0;
  int cols = 64;
  struct Mailbox *mailbox = mailbox_new();
  mailbox->name = mutt_str_dup("MailBox");
  mailbox->pathbuf = mutt_buffer_make(16);
  mailbox->msg_unread = 7;
  mutt_buffer_strcpy(&mailbox->pathbuf, "/path");

  struct EventMailbox ev_m = { mailbox, ARRAY_HEAD_INITIALIZER };
  ARRAY_ADD(&ev_m.emails, email_new());
  ARRAY_ADD(&ev_m.emails, email_new());

  intptr_t data = (intptr_t) &ev_m;

  new_mail_format_str((char *) buf, 64, col, cols, 'n', NULL, NULL, NULL, NULL, data, 0);
  TEST_CHECK(mutt_str_equal(buf, "MailBox"));
  TEST_MSG("Check failed: %s != MailBox", buf);

  new_mail_format_str((char *) buf, 64, col, cols, 'f', NULL, NULL, NULL, NULL, data, 0);
  TEST_CHECK(mutt_str_equal(buf, "/path"));
  TEST_MSG("Check failed: %s != /path", buf);

  new_mail_format_str((char *) buf, 64, col, cols, 'u', NULL, NULL, NULL, NULL, data, 0);
  TEST_CHECK(mutt_str_equal(buf, "7"));
  TEST_MSG("Check failed: %s != 7", buf);

  new_mail_format_str((char *) buf, 64, col, cols, 'c', NULL, NULL, NULL, NULL, data, 0);
  TEST_CHECK(mutt_str_equal(buf, "2"));
  TEST_MSG("Check failed: %s != 2", buf);

  struct Email **email;
  ARRAY_FOREACH(email, &ev_m.emails)
  {
    email_free(email);
  }
  ARRAY_FREE(&ev_m.emails);
  mailbox_free(&mailbox);
}
