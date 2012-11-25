<?php
/**
 * @file
 * Defines a class that overrides some methods of the Twitter libray.
 */

/**
 * Primary Twitter API implementation class
 */
class Twitter6 extends Twitter {
  /**
   * Actually performs a request.
   *
   * This method can be easily overriden through inheritance.
   *
   * @param string $url
   *   The url of the endpoint.
   * @param array $headers
   *   Array of headers.
   * @param string $method
   *   The HTTP method to use (normally POST or GET).
   * @param array $data
   *   An array of parameters
   * @return
   *   stdClass response object.
   */
  protected function doRequest($url, $headers, $method, $data) {
    return drupal_http_request($url, $headers, $method, $data);
  }
}
